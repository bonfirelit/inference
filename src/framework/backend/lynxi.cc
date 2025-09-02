#include "backend/lynxi.h"

Lynxi::Lynxi(int dev_id) : Backend(BACKEND_LYNXI, dev_id) {
    // 不setDevice也能用
    // lynSetDevice(dev_id);
}

Lynxi::~Lynxi() {}


Result Lynxi::init() {
    if (ctx_ != nullptr) { // 该backend已经初始化过了
        return SUCCESS;
    }
    assert(ctx_ == nullptr);
    lynError_t err = lynCreateContext(&ctx_, device_id_);
    assert(ctx_ != nullptr);
    if (err != 0) {
        ERROR_LOG("lynxi create context failed, id[%d]", device_id_);
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::finalize() {
    lynSetCurrentContext(ctx_);
    INFO_LOG("lynxi backend finalize start");
    if (ctx_ == nullptr) {
        INFO_LOG("lynxi backend finalize: ctx_ is null, skip");
        return SUCCESS;
    }
    auto err = lynDestroyContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynxi destory context failed");
        return FAIL;
    }
    ctx_ = nullptr;
    INFO_LOG("lynxi backend finalize end");
    return SUCCESS;
}

Result Lynxi::memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
    lynSetCurrentContext(ctx_);
    lynError_t err;
    if (dir == DEVICE2HOST) {
        err = lynMemcpy(dst, src, size, ServerToClient);
    } else if (dir == HOST2DEVICE) {
        err = lynMemcpy(dst, src, size, ClientToServer);
    } else {
        ERROR_LOG("unkown copy direction");
        return FAIL;
    }
    if (err != 0) {
        ERROR_LOG("lynxi memcpy failed");
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::malloc(void **dev_ptr, uint64_t size) {
    lynSetCurrentContext(ctx_);
    lynError_t err = lynMalloc(dev_ptr, size);
    if (err != 0) {
        ERROR_LOG("lynxi malloc failed");
        dev_ptr = nullptr;
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::free(void *dev_ptr) {
    lynSetCurrentContext(ctx_);
    lynError_t err = lynFree(dev_ptr);
    if (err != 0) {
        ERROR_LOG("lynxi free failed");
        return FAIL;
    }
    return SUCCESS;
}

// 加载模型，同时创建modelinfo，返回model_id
int Lynxi::loadModel(const std::string &path) {
    lynSetCurrentContext(ctx_);
    // 判断模型是否已加载
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it = path_to_id_.find(path);
        if (it != path_to_id_.end()) {
            return it->second;
        }
    }
    // load model
    lynModel_t model;
    lynError_t err = lynLoadModel(path.c_str(), &model); // loadModel是线程安全吗
    if (err != 0) {
        ERROR_LOG("lynxi load model failed");
        return -1;
    }

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it = path_to_id_.find(path);
        if (it != path_to_id_.end()) {
            lynUnloadModel(model);
            return it->second;
        }

        int model_id = next_model_id_++;

        lynModelDesc_t *model_desc = nullptr;
        err = lynModelGetDesc(model, &model_desc);
        if (err != 0) {
            lynUnloadModel(model);
            ERROR_LOG("lynxi get model desc failed");
            return -1;
        }
        assert(model_desc->inputDataLen != 0);
        assert(model_desc->outputDataLen != 0);

        size_t batch_size = model_desc->inputTensorAttrArray->batchSize;
        size_t input_size, output_size;
        uint32_t input_num, output_num;
        lynDataType_t in_type, out_type;
        lynModelGetInputDataTotalLen(model, &input_size);
        lynModelGetOutputDataTotalLen(model, &output_size);
        lynModelGetOutputTensorNum(model, &output_num);
        lynModelGetInputTensorNum(model, &input_num);
        // 假设多个输入张量的数据类型相同，输出张量也是
        lynModelGetInputTensorDataTypeByIndex(model, 0, &in_type);
        lynModelGetOutputTensorDataTypeByIndex(model, 0, &out_type);
        
        std::vector<std::vector<uint32_t>> outs_dim;
        for (int i = 0; i < output_num; i++) {
            uint32_t dim_count = 0;
            std::vector<uint32_t> dim(sizeof(uint32_t) * LYN_MAX_DIMS_COUNT);
            lynModelGetOutputTensorDimsByIndex(model, i, dim.data(), &dim_count);
            dim.resize(dim_count);
            outs_dim.emplace_back(std::move(dim));
        }
        std::vector<std::vector<uint32_t>> ins_dim;
        for (int i = 0; i < input_num; i++) {
            uint32_t dim_count = 0;
            std::vector<uint32_t> dim(sizeof(uint32_t) * LYN_MAX_DIMS_COUNT);
            lynModelGetInputTensorDimsByIndex(model, i, dim.data(), &dim_count);
            dim.resize(dim_count);
            ins_dim.emplace_back(std::move(dim));
        }

        path_to_id_[path] = model_id;
        models_[model_id] = std::make_unique<LynxiModel>(this, model);
        infos_[model_id] = std::make_unique<ModelInfo>(
            batch_size,
            input_size,
            output_size,
            input_num,
            output_num,
            std::move(ins_dim),
            std::move(outs_dim),
            this->convertDataType(in_type),
            this->convertDataType(out_type)
        );

        return model_id;
    }
}

Result Lynxi::unloadModel(const std::string& path) {
    lynSetCurrentContext(ctx_);
    lynModel_t model;
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (path_to_id_.find(path) == path_to_id_.end()) {
            return SUCCESS;
        }
        int model_id = path_to_id_[path];
        auto it = models_.find(model_id);
        if (it == models_.end()) {
            ERROR_LOG("can't find model, unload failed");
            return FAIL;
        }
        model = (lynModel_t)it->second->getHandle();

        path_to_id_.erase(path);
        models_.erase(model_id);
        infos_.erase(model_id);
    }
    auto err = lynUnloadModel(model);
    if (err != 0) {
        ERROR_LOG("lynxi unloadmodel failed");
        return FAIL;
    }
    return SUCCESS;
}

std::vector<Tensor> Lynxi::infer(Stream* stream, int model_id, std::vector<Tensor>&& inputs) {
    lynStream_t lynstream = stream->getStream();
    lynModel_t model;
    size_t batch_size;
    uint32_t input_num, output_num;
    size_t batch_input_size, batch_output_size;
    std::vector<std::vector<uint32_t>> shapes;
    DataType output_type;
    lynSetCurrentContext(ctx_);

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it_model = models_.find(model_id);
        auto it_info  = infos_.find(model_id);
        if (it_model == models_.end() || it_info == infos_.end()) {
            ERROR_LOG("lynxi infer(): model or model_info doesn't exist");
            return {};
        }
        model = (lynModel_t)it_model->second->getHandle();
        batch_size = it_info->second->getBatchSize();
        input_num = it_info->second->getInputNum();
        output_num = it_info->second->getOutputNum();
        batch_input_size = it_info->second->getInputSize();
        batch_output_size = it_info->second->getOutputSize();
        shapes = it_info->second->getOutputsShape();
        output_type = it_info->second->getOutputType();
    }
    assert(input_num == inputs.size());

    // Prepare Input
    INFO_LOG("lynxi infer(): prepare input start");
    size_t input_size = 0;
    for (auto& tensor : inputs) {
        input_size += tensor.size();
    }
    size_t model_input_size = batch_size * batch_input_size;
    if (input_size != model_input_size) {
        ERROR_LOG("lynxi infer(): input size[%zu] does not match model input size[%zu]", 
                    input_size, model_input_size);
        return {};
    }
    void* dev_input_ptr = nullptr;
    this->malloc((void**)&dev_input_ptr, input_size);
    assert(dev_input_ptr != nullptr);

    auto temp = static_cast<char*>(dev_input_ptr);
    for (auto& tensor : inputs) {
        auto size = tensor.size();
        this->memcopy(temp, tensor.data(), size, HOST2DEVICE);
        temp += size;
    }

    // Prepare Output
    INFO_LOG("lynxi infer(): prepare output start");
    void* dev_output_ptr = nullptr;
    size_t model_output_size = batch_size * batch_output_size;
    this->malloc((void**)&dev_output_ptr, model_output_size);
    assert(dev_output_ptr != nullptr);

    INFO_LOG("lynxi infer(): model inference start");
    lynExecuteModelAsync(lynstream, model, dev_input_ptr, dev_output_ptr, batch_size);
    lynSynchronizeStream(lynstream);

    // Copy to host
    INFO_LOG("lynxi infer(): copy to host start");
    std::vector<Tensor> outputs;
    outputs.reserve(output_num);
    assert(shapes.size() == output_num);
    auto dev_out_ptr = static_cast<const char*>(dev_output_ptr);
    for (int i = 0; i < output_num; i++) {
        outputs.emplace_back(shapes[i], output_type);
        Tensor& tensor = outputs.back();
        size_t output_size = getElementSize(output_type) * std::accumulate(
            shapes[i].begin(), shapes[i].end(), 1u, std::multiplies<uint32_t>()
            );
        assert(tensor.size() == output_size);
        assert(tensor.data() != nullptr);
        auto err = this->memcopy(
            tensor.data(),
            dev_out_ptr + i * output_size,
            output_size,
            DEVICE2HOST
        );
        if (err != SUCCESS) {
            ERROR_LOG("lynxi infer(): copy to host failed");
            return {};
        }
    }

    // free device mem
    INFO_LOG("lynxi infer(): free device mem start");
    this->free(dev_input_ptr);
    this->free(dev_output_ptr);

    return outputs;
}



const ModelInfo* Lynxi::getModelInfo(int model_id) const {
    std::lock_guard<std::mutex> lock(model_lock_);
    auto it = infos_.find(model_id);
    if (it == infos_.end()) {
        ERROR_LOG("ModelInfo doesn't exist");
        return nullptr;
    }
    return it->second.get();
}

std::unique_ptr<Stream> Lynxi::createStream() {
    auto err = lynSetCurrentContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynSetCurrentContext Fail");
        return std::make_unique<LynxiStream>(this, ctx_);
    }
    
    std::unique_ptr<LynxiStream> stream = std::make_unique<LynxiStream>(this, ctx_);
    stream->createStream();
    return stream;
}

Result Lynxi::destoryStream(Stream* stream) {
    lynSetCurrentContext(ctx_);
    return stream->destoryStream();
}

void* LynxiModel::getHandle() {
    return model_;
}

Result LynxiStream::synchronize() {
    lynSetCurrentContext(ctx_);
    auto err = lynSynchronizeStream(stream_);
    if (err != 0) {
        ERROR_LOG("lynxi synchronize stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::createStream() {
    lynSetCurrentContext(ctx_);
    auto err = lynCreateStream(&stream_);
    if (err != 0) {
        ERROR_LOG("lynxi create stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::destoryStream() {
    lynSetCurrentContext(ctx_);
    auto err = lynDestroyStream(stream_);
    if (err != 0) {
        ERROR_LOG("lynxi destory stream failed!");
        return FAIL;
    }
    stream_ = nullptr;
    return SUCCESS;
}

Result LynxiStream::recordEvent(Event* event) {
    lynSetCurrentContext(ctx_);
    auto err = lynRecordEvent(stream_, (lynEvent_t)event->getEvent());
    if (err != 0) {
        ERROR_LOG("lynxi record event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::waitEvent(Event* event) {
    lynSetCurrentContext(ctx_);
    auto err = lynStreamWaitEvent(stream_, (lynEvent_t)event->getEvent());
    if (err != 0) {
        ERROR_LOG("lynxi wait event failed!");
        return FAIL;
    }
    return SUCCESS;
}

void* LynxiStream::getStream() {
    return stream_;
}

Result LynxiEvent::createEvent() {
    auto err = lynCreateEvent(&event_);
    if (err != 0) {
        ERROR_LOG("lynxi create event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiEvent::destoryEvent() {
    auto err = lynDestroyEvent(event_);
    if (err != 0) {
        ERROR_LOG("lynxi destory event failed!");
        return FAIL;
    }
    event_ = nullptr;
    return SUCCESS;
}

Result LynxiEvent::synchronize() {
    auto err = lynSynchronizeEvent(event_);
    if (err != 0) {
        ERROR_LOG("lynxi synchronize event failed!");
        return FAIL;
    }
    return SUCCESS;
}

void* LynxiEvent::getEvent() {
    return event_;
}

DataType Lynxi::convertDataType(lynDataType_t dt) {
    switch (dt) {
        case DT_FLOAT:   return FLOAT32;
        case DT_INT8:    return INT8;
        case DT_UINT8:   return UINT8;
        case DT_FLOAT16: return FLOAT16;
        default:         return UNKNOWN;
    }
}