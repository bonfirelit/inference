#include "backend/lynxi.h"

Lynxi::Lynxi(int dev_id) : device_id_(dev_id) {

}

Lynxi::~Lynxi() {
    lynDestroyContext(ctx_);
}

// 初始化设备。对于灵汐是为设备创建ctx
Result Lynxi::Init() {
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

// 有用吗???????????????????
Result Lynxi::Finalize() {
    auto err = lynDestroyContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynxi销毁ctx失败");
        return FAIL;
    }
    return SUCCESS;
}

Result Lynxi::MemCopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
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

void Lynxi::Malloc(void **dev_ptr, uint64_t size) {
    lynError_t err = lynMalloc(dev_ptr, size);
    if (err != 0) {
        ERROR_LOG("lynxi: 分配内存失败");
        dev_ptr = nullptr;
    }
}

void Lynxi::Free(void *dev_ptr) {
    lynError_t err = lynFree(dev_ptr);
    if (err != 0) {
        ERROR_LOG("lynxi: 释放内存失败");
    }
}

// 加载模型，同时创建modelinfo，返回model_id
int Lynxi::LoadModel(const std::string &path) {

    // 判断模型是否已加载
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it = path2id_.find(path);
        if (it != path2id_.end()) {
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
        auto it = path2id_.find(path);
        if (it != path2id_.end()) {
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

        path2id_[path] = model_id;
        models_[model_id] = model;
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

Result Lynxi::UnloadModel(const std::string& path) {
    lynModel_t model;
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (path2id_.find(path) == path2id_.end()) {
            return SUCCESS;
        }
        int model_id = path2id_[path];
        auto it = models_.find(model_id);
        if (it == models_.end()) {
            ERROR_LOG("未找到模型句柄,unload失败");
            return FAIL;
        }
        model = it->second;

        path2id_.erase(path);
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

Result Lynxi::Infer(Executor* e, int model_id, void* dev_input_ptr, void* dev_output_ptr) {
    lynStream_t stream;
    lynModel_t model;
    size_t batch_size;

    {
        std::lock_guard<std::mutex> lock(stream_lock_);
        auto it = executor2stream_.find(e);
        if (it == executor2stream_.end()) {
            ERROR_LOG("executor对应stream不存在");
            return FAIL;
        }
        stream = it->second;
    }

    {
        std::lock_guard<std::mutex> lock(model_lock_);
        auto it_model = models_.find(model_id);
        auto it_info  = infos_.find(model_id);
        if (it_model == models_.end() || it_info == infos_.end()) {
            ERROR_LOG("model或model_info不存在");
            return FAIL;
        }
        model = it_model->second;
        batch_size = it_info->second->getBatchSize();
    }

    lynExecuteModelAsync(stream, model, dev_input_ptr, dev_output_ptr, batch_size);
    lynSynchronizeStream(stream);
    return SUCCESS;
}

// 如果涉及到executor的动态扩容，这些函数读取了map，也需要加锁
const ModelInfo* Lynxi::GetModelInfo(int model_id) const {
    std::lock_guard<std::mutex> lock(model_lock_);
    auto it = infos_.find(model_id);
    if (it == infos_.end()) {
        ERROR_LOG("ModelInfo不存在");
        return nullptr;
    }
    return it->second.get();
}

// 为执行器创建运行时资源。对于灵汐是创建Stream
Result Lynxi::InitRtResource(Executor* e) {
    auto err = lynSetCurrentContext(ctx_);
    if (err != 0) {
        ERROR_LOG("lynSetCurrentContext Fail");
        return FAIL;
    }

    lynStream_t stream = nullptr;
    err = lynCreateStream(&stream);
    if (err != 0) {
        ERROR_LOG("lynxi创建Stream失败");
        return FAIL;
    }

    {
        std::lock_guard<std::mutex> lock(stream_lock_);    
        executor2stream_[e] = stream;
    }
    return SUCCESS;
}

Result Lynxi::FinalizeRtResource(Executor* e) {
    lynStream_t stream;
    {
        std::lock_guard<std::mutex> lock(stream_lock_);
        auto it = executor2stream_.find(e);
        if (it != executor2stream_.end()) {
            stream = it->second;
            executor2stream_.erase(it);
        } else {
            ERROR_LOG("stream不存在");
            return FAIL;
        }
    }
    auto err = lynDestroyStream(stream);
    if (err != 0) {
        ERROR_LOG("销毁stream失败");
        return FAIL;
    }
    return SUCCESS;
}