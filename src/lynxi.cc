#include "backend/lynxi.h"

Lynxi::Lynxi(int dev_id) : Backend(BACKEND_LYNXI, dev_id) {}

Lynxi::~Lynxi() {
    // ?????
    lynDestroyContext(ctx_);
}


Result Lynxi::init() {
    if (ctx_ != nullptr) { // 该backend已经初始化过了
        return SUCCESS;
    }
    lynError_t err = lynCreateContext(&ctx_, device_id_);
    if (err != 0) {
        ERROR_LOG("lynxi创建ctx失败,设备id为%d", device_id_);
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::finalize() {
    auto err = lynDestroyContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynxi销毁ctx失败");
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
    lynError_t err;
    if (dir == DEVICE2HOST) {
        err = lynMemcpy(dst, src, size, ServerToClient);
    } else if (dir == HOST2DEVICE) {
        err = lynMemcpy(dst, src, size, ClientToServer);
    } else {
        ERROR_LOG("未知的内存拷贝方向");
        return FAIL;
    }
    if (err != 0) {
        ERROR_LOG("内存拷贝失败");
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::malloc(void **dev_ptr, uint64_t size) {
    lynError_t err = lynMalloc(dev_ptr, size);
    if (err != 0) {
        ERROR_LOG("lynxi: 分配内存失败");
        dev_ptr = nullptr;
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::free(void *dev_ptr) {
    lynError_t err = lynFree(dev_ptr);
    if (err != 0) {
        ERROR_LOG("lynxi: 释放内存失败");
        return FAIL;
    }
    return SUCCESS;
}

// 加载模型，同时创建modelinfo，返回model_id
uint32_t Lynxi::loadModel(const std::string &path) {

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
        ERROR_LOG("lynxi: 加载模型错误");
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
            ERROR_LOG("lynxi: 获取modelDesc失败");
            return -1;
        }
        assert(model_desc->inputDataLen != 0);
        assert(model_desc->outputDataLen != 0);

        size_t batch_size = model_desc->inputTensorAttrArray->batchSize;
        size_t input_size, output_size;
        uint32_t input_num, output_num;
        lynModelGetInputDataTotalLen(model, &input_size);
        lynModelGetOutputDataTotalLen(model, &output_size);
        lynModelGetOutputTensorNum(model, &output_num);
        lynModelGetInputTensorNum(model, &input_num);
        
        std::vector<std::vector<uint32_t>> outs_dim;
        for (int i = 0; i < output_num; i++) {
            uint32_t dim_count = 0;
            lynModelGetOutputTensorDimsByIndex(model, i, nullptr, &dim_count);
            assert(dim_count > 0);
            std::vector<uint32_t> dim(dim_count);
            lynModelGetOutputTensorDimsByIndex(model, i, dim.data(), &dim_count);
            outs_dim.emplace_back(std::move(dim));
        }
        std::vector<std::vector<uint32_t>> ins_dim;
        for (int i = 0; i < input_num; i++) {
            uint32_t dim_count = 0;
            lynModelGetInputTensorDimsByIndex(model, i, nullptr, &dim_count);
            assert(dim_count > 0);
            std::vector<uint32_t> dim(dim_count);
            lynModelGetInputTensorDimsByIndex(model, i, dim.data(), &dim_count);
            ins_dim.emplace_back(std::move(dim));
        }

        path_to_id_[path] = model_id;
        models_[model_id] = std::make_unique<LynxiModel>(this);
        infos_[model_id] = std::make_unique<ModelInfo>(
            batch_size,
            input_size,
            output_size,
            input_num,
            output_num,
            std::move(ins_dim),
            std::move(outs_dim)
        );

        return model_id;
    }
}

Result Lynxi::unloadModel(const std::string& path) {
    lynModel_t model;
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (path_to_id_.find(path) == path_to_id_.end()) {
            return SUCCESS;
        }
        int model_id = path_to_id_[path];
        auto it = models_.find(model_id);
        if (it == models_.end()) {
            ERROR_LOG("未找到模型句柄,unload失败");
            return FAIL;
        }
        model = (lynModel_t)it->second->getHandle();

        path_to_id_.erase(path);
        models_.erase(model_id);
        infos_.erase(model_id);
    }
    auto err = lynUnloadModel(model);
    if (err != 0) {
        ERROR_LOG("lynxi: 卸载模型失败");
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::infer(Executor* e, uint32_t model_id, void* dev_input_ptr, void* dev_output_ptr) {
    lynStream_t stream;
    lynModel_t model;
    size_t batch_size;

    {
        std::lock_guard<std::mutex> lock(stream_lock_);
        auto it = exec_to_stream_.find(e);
        if (it == exec_to_stream_.end()) {
            ERROR_LOG("executor对应stream不存在");
            return FAIL;
        }
        stream = (lynStream_t)it->second->getStream();
    }

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it_model = models_.find(model_id);
        auto it_info  = infos_.find(model_id);
        if (it_model == models_.end() || it_info == infos_.end()) {
            ERROR_LOG("model或model_info不存在");
            return FAIL;
        }
        model = (lynModel_t)it_model->second->getHandle();
        batch_size = it_info->second->getBatchSize();
    }

    lynExecuteModelAsync(stream, model, dev_input_ptr, dev_output_ptr, batch_size);
    lynSynchronizeStream(stream);
    return SUCCESS;
}

const ModelInfo* Lynxi::getModelInfo(uint32_t model_id) const {
    std::lock_guard<std::mutex> lock(model_lock_);
    auto it = infos_.find(model_id);
    if (it == infos_.end()) {
        ERROR_LOG("ModelInfo不存在");
        return nullptr;
    }
    return it->second.get();
}

Result Lynxi::createStream(Executor* e) {
    auto err = lynSetCurrentContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynSetCurrentContext Fail");
        return FAIL;
    }

    std::unique_ptr<LynxiStream> stream = std::make_unique<LynxiStream>(this);
    Result res = stream->createStream();
    if (res == FAIL) {
        return res;
    }

    {
        std::lock_guard<std::mutex> lock(stream_lock_);    
        exec_to_stream_[e] = std::move(stream);
    }
    return SUCCESS;
}

Result Lynxi::destoryStream(Executor* e) {
    std::lock_guard<std::mutex> lock(stream_lock_);
    auto it = exec_to_stream_.find(e);
    Result res = SUCCESS;
    if (it != exec_to_stream_.end()) {
        res = it->second->destoryStream();
        exec_to_stream_.erase(it);
    } else {
        ERROR_LOG("stream不存在");
        return FAIL;
    }
    return res;
}

void* LynxiModel::getHandle() {
    return model_;
}

Result LynxiStream::synchronize() {
    auto err = lynSynchronizeStream(stream_);
    if (err != 0) {
        ERROR_LOG("lynxi synchronize stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::createStream() {
    auto err = lynCreateStream(&stream_);
    if (err != 0) {
        ERROR_LOG("lynxi create stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::destoryStream() {
    auto err = lynDestroyStream(stream_);
    if (err != 0) {
        ERROR_LOG("lynxi destory stream failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::recordEvent(Event* event) {
    auto err = lynRecordEvent(stream_, (lynEvent_t)event->getEvent());
    if (err != 0) {
        ERROR_LOG("lynxi record event failed!");
        return FAIL;
    }
    return SUCCESS;
}

Result LynxiStream::waitEvent(Event* event) {
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