#include "backend/ascend.h"

Ascend::Ascend(int dev_id) : Backend(BACKEND_ACL, dev_id) {
    aclError ret = aclInit(nullptr);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend init failed");
    }
    ret = aclrtSetDevice(dev_id);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend set device failed");
    }
}

Ascend::~Ascend() {
    auto ret = aclrtResetDevice(device_id_);
    if (ret != ACL_SUCCESS){
        ERROR_LOG("ascend reset device failed");
    }
    ret = aclFinalize();
    if (ret != ACL_SUCCESS){
        ERROR_LOG("ascend finalize failed");
    }
}

Result Ascend::init() {
    if (ctx_ != nullptr) {
        return SUCCESS;
    }
    auto err = aclrtCreateContext(&ctx_, device_id_);
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend create context failed, id[%d]", device_id_);
        return FAIL;
    }
    return SUCCESS;
}

Result Ascend::finalize() {
    auto err = aclrtDestroyContext(ctx_);
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend destory context failed");
        return FAIL;
    }
    return SUCCESS;
}

Result Ascend::memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
    aclError err;
    if (dir == DEVICE2HOST) {
        err = aclrtMemcpy(dst, size, src, size, ACL_MEMCPY_DEVICE_TO_HOST);
    } else if (dir == HOST2DEVICE) {
        err = aclrtMemcpy(dst, size, src, size, ACL_MEMCPY_HOST_TO_DEVICE);
    } else {
        ERROR_LOG("unkown copy direction");
        return FAIL;
    }
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend memcpy failed");
        return FAIL;
    }
    return SUCCESS;
}

Result Ascend::malloc(void **dev_ptr, uint64_t size) {
    aclError err = aclrtMalloc(dev_ptr, size, ACL_MEM_MALLOC_HUGE_FIRST);
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend malloc failed");
        dev_ptr = nullptr;
        return FAIL;
    }
    return SUCCESS;
}

Result Ascend::free(void *dev_ptr) {
    aclError err = aclrtFree(dev_ptr);
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend free failed");
        return FAIL;
    }
    return SUCCESS;
}

int Ascend::loadModel(const std::string &path) {
    // 判断模型是否已加载
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it = path_to_id_.find(path);
        if (it != path_to_id_.end()) {
            return it->second;
        }
    }
    uint32_t ascend_model;
    aclError err = aclmdlLoadFromFile(path.c_str(), &ascend_model); // loadModel是线程安全吗
    if (err != ACL_SUCCESS) {
        ERROR_LOG("ascend load model failed");
        return -1;
    }

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it = path_to_id_.find(path);
        if (it != path_to_id_.end()) {
            aclmdlUnload(ascend_model);
            return it->second;
        }

        int model_id = next_model_id_++;

        aclmdlDesc *model_desc = aclmdlCreateDesc();
        if (model_desc == nullptr) {
            ERROR_LOG("create model description failed");
            aclmdlUnload(ascend_model);
            return -1;
        }
        aclError ret = aclmdlGetDesc(model_desc, ascend_model);
        if (ret != ACL_SUCCESS) {
            ERROR_LOG("get model description failed, ascend_model_id is %u, errorCode is %d",
            ascend_model, static_cast<int32_t>(ret));
            aclmdlUnload(ascend_model);
            return -1;
        }

        // ascend don't need batch size
        size_t batch_size = 0;
        size_t input_size = 0, output_size = 0;

        uint32_t input_num = aclmdlGetNumInputs(model_desc);
        uint32_t output_num = aclmdlGetNumOutputs(model_desc);
        for (size_t i = 0; i < input_num; i++) {
            input_size += aclmdlGetInputSizeByIndex(model_desc, i);
        }
        for (size_t i = 0; i < output_num; i++) {
            output_size += aclmdlGetOutputSizeByIndex(model_desc, i);
        }
        // 假设多个输入张量的数据类型相同，输出张量也是
        aclDataType in_type = aclmdlGetInputDataType(model_desc, 0);
        aclDataType out_type = aclmdlGetOutputDataType(model_desc, 0);

        std::vector<std::vector<uint32_t>> outs_dim;
        for (int i = 0; i < output_num; i++) {
            aclmdlIODims out;
            aclmdlGetOutputDims(model_desc, i, &out);
            std::vector<uint32_t> dim;
            dim.reserve(out.dimCount);
            for (size_t i = 0; i < out.dimCount; i++) {
                dim.push_back(static_cast<uint32_t>(out.dims[i]));
            }
            outs_dim.emplace_back(std::move(dim));
        }
        std::vector<std::vector<uint32_t>> ins_dim;
        for (int i = 0; i < input_num; i++) {
            aclmdlIODims in;
            aclmdlGetOutputDims(model_desc, i, &in);
            std::vector<uint32_t> dim;
            dim.reserve(in.dimCount);
            for (size_t i = 0; i < in.dimCount; i++) {
                dim.push_back(static_cast<uint32_t>(in.dims[i]));
            }
            ins_dim.emplace_back(std::move(dim));
        }

        path_to_id_[path] = model_id;
        models_[model_id] = std::make_unique<AscendModel>(this, ascend_model);
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

        aclmdlDestroyDesc(model_desc);

        return model_id;
    }
}

Result Ascend::unloadModel(const std::string& path) {
    uint32_t ascend_model;
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (path_to_id_.find(path) == path_to_id_.end()) {
            return SUCCESS;
        }
        uint32_t model_id = path_to_id_[path];
        auto it = models_.find(model_id);
        if (it == models_.end()) {
            ERROR_LOG("ascend unload failed");
            return FAIL;
        }
        ascend_model = *(uint32_t*)it->second->getHandle();

        path_to_id_.erase(path);
        models_.erase(model_id);
        infos_.erase(model_id);
    }
    auto err = aclmdlUnload(ascend_model);
    if (err != 0) {
        ERROR_LOG("acl unload model failed");
        return FAIL;
    }
    return SUCCESS;
}

/**
 * 对同一个modelId的模型，不能调用aclmdlExecuteAsync接口执行多Stream并发场景下的模型推理！
 * 错误示例如下：
 *  aclmdlExecuteAsync(modelId1, input, output, stream1);
 *  aclmdlExecuteAsync(modelId1, input, output, stream2);
 *  aclrtSynchronizeStream(stream1);
 *  aclrtSynchronizeStream(stream2);
 * 对于多个executor，会出错，lynxi那边可能也一样
 */
std::vector<Tensor> Ascend::infer(Stream* stream, int model_id, std::vector<Tensor>&& inputs) {
    aclrtStream aclstream = stream->getStream();
    uint32_t model;
    uint32_t input_num, output_num;
    std::vector<std::vector<uint32_t>> output_shapes;
    DataType output_type;

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it_model = models_.find(model_id);
        auto it_info  = infos_.find(model_id);
        if (it_model == models_.end() || it_info == infos_.end()) {
            ERROR_LOG("ascend infer(): model or model_info doesn't exist");
            return {};
        }
        model = *(uint32_t*)it_model->second->getHandle();
        input_num = it_info->second->getInputNum();
        output_num = it_info->second->getOutputNum();
        output_shapes = it_info->second->getOutputsShape();
        output_type = it_info->second->getOutputType();
    }
    assert(input_num == inputs.size());
    assert(output_shapes.size() == output_num);

    aclmdlDesc *model_desc = aclmdlCreateDesc();
    if (model_desc == nullptr) {
        ERROR_LOG("create model description failed");
        return {};
    }
    aclError ret = aclmdlGetDesc(model_desc, model);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("get model description failed, ascend_model_id is %u, errorCode is %d",
        model, static_cast<int32_t>(ret));
        return {};
    }

    // Prepare input
    INFO_LOG("ascend infer(): prepare input start");
    aclmdlDataset* input = aclmdlCreateDataset();
    for (size_t i = 0; i < input_num; i++) {
        void* input_buffer = nullptr;
        size_t input_size = aclmdlGetInputSizeByIndex(model_desc, i);
        assert(input_size == inputs[i].size());
        // malloc on device
        this->malloc(&input_buffer, input_size);
        // copy to device
        this->memcopy(input_buffer, inputs[i].data(), input_size, DEVICE2HOST);
        aclDataBuffer *input_data = aclCreateDataBuffer(input_buffer, input_size);
        aclmdlAddDatasetBuffer(input, input_data);
    }

    // Prepare output
    INFO_LOG("ascend infer(): prepare output start");
    aclmdlDataset* output = aclmdlCreateDataset();
    for (size_t i = 0; i < output_num; i++) {
        void* output_buffer = nullptr;
        size_t output_size = aclmdlGetOutputSizeByIndex(model_desc, i);
        // malloc on device
        this->malloc(&output_buffer, output_size);
        aclDataBuffer *output_data = aclCreateDataBuffer(output_buffer, output_size);
        aclmdlAddDatasetBuffer(output, output_data);
    }

    INFO_LOG("ascend infer(): model inference start");
    aclmdlExecuteAsync(model, input, output, aclstream);
    aclrtSynchronizeStream(aclstream);

    // Copy to host
    INFO_LOG("ascend infer(): copy output to host start");
    std::vector<Tensor> ret_tensor;
    ret_tensor.reserve(output_num);
    for (size_t i = 0; i < aclmdlGetDatasetNumBuffers(output); i++) {
        aclDataBuffer* dataBuffer = aclmdlGetDatasetBuffer(output, i);
        void* device_data = aclGetDataBufferAddr(dataBuffer);
        size_t len = aclGetDataBufferSizeV2(dataBuffer);
        ret_tensor.emplace_back(output_shapes[i], output_type);
        Tensor& tensor = ret_tensor.back();
        size_t output_size = getElementSize(output_type) * std::accumulate(
            output_shapes[i].begin(), output_shapes[i].end(), 1u, std::multiplies<uint32_t>()
            );
        assert(tensor.size() == output_size);
        assert(len == output_size);
        assert(tensor.data() != nullptr);
        auto err = this->memcopy(
            tensor.data(),
            device_data,
            output_size,
            DEVICE2HOST
        );
        if (err != SUCCESS) {
            ERROR_LOG("ascend infer(): copy to host failed");
            return {};
        }
    }

    // free device mem
    INFO_LOG("ascend infer(): free device mem start");
    for (size_t i = 0; i < aclmdlGetDatasetNumBuffers(input); ++i) {
        aclDataBuffer *dataBuffer = aclmdlGetDatasetBuffer(input, i);
        void* device_data = aclGetDataBufferAddr(dataBuffer);
        this->free(device_data);
        (void)aclDestroyDataBuffer(dataBuffer);
    }
    (void)aclmdlDestroyDataset(input);
    input = nullptr;

    for (size_t i = 0; i < aclmdlGetDatasetNumBuffers(output); ++i) {
        aclDataBuffer *dataBuffer = aclmdlGetDatasetBuffer(output, i);
        void* device_data = aclGetDataBufferAddr(dataBuffer);
        this->free(device_data);
        (void)aclDestroyDataBuffer(dataBuffer);
    }
    (void)aclmdlDestroyDataset(output);
    output = nullptr;


    return ret_tensor;
}

const ModelInfo* Ascend::getModelInfo(int model_id) const {
    std::lock_guard<std::mutex> lock(model_lock_);
    auto it = infos_.find(model_id);
    if (it == infos_.end()) {
        ERROR_LOG("ModelInfo doesn't exist");
        return nullptr;
    }
    return it->second.get();
}

std::unique_ptr<Stream> Ascend::createStream() {
    auto ret = aclrtSetCurrentContext(ctx_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtSetCurrentContext Fail");
        return std::make_unique<AscendStream>(this);
    }
    
    std::unique_ptr<AscendStream> stream = std::make_unique<AscendStream>(this);
    stream->createStream();
    return stream;
}

Result Ascend::destoryStream(Stream* stream) {
    return stream->destoryStream();
}

void* AscendModel::getHandle() {
    return (void*)&model_;
}

Result AscendStream::synchronize() {
    auto ret = aclrtSynchronizeStream(stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend synchronize stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendStream::createStream() {
    auto ret = aclrtCreateStream(&stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend create stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendStream::destoryStream() {
    auto ret = aclrtDestroyStream(stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend destory stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendStream::recordEvent(Event* event) {
    auto ret = aclrtRecordEvent((aclrtEvent)event->getEvent(), stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend record event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendStream::waitEvent(Event* event) {
    auto ret = aclrtStreamWaitEvent(stream_, (aclrtEvent)event->getEvent());
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend wait event failed!");
        return FAIL;
    }
    return SUCCESS;
}

void* AscendStream::getStream() {
    return stream_;
}

Result AscendEvent::createEvent() {
    auto ret = aclrtCreateEvent(&event_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend create event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendEvent::destoryEvent() {
    auto ret = aclrtDestroyEvent(event_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend destory event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result AscendEvent::synchronize() {
    auto ret = aclrtSynchronizeEvent(event_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("ascend synchronize event failed!");
        return FAIL;
    }
    return SUCCESS;
}

void* AscendEvent::getEvent() {
    return event_;
}

DataType Ascend::convertDataType(aclDataType dt) {
    switch (dt) {
        case ACL_FLOAT:    return FLOAT32;
        case ACL_FLOAT16:  return FLOAT16;
        case ACL_INT8:     return INT8;
        case ACL_UINT8:    return UINT8;
        default:           return UNKNOWN;
    }
}
