#include "executor.h"

Executor::Executor(const std::string& model_path, Backend* backend, TaskQueue* tq, int id) 
    : backend_(backend)
    , model_path_(model_path)
    , tq_(tq)
    , id_(id) {
        INFO_LOG("executor[%d] created!", id_);
    }

Executor::~Executor() {
    INFO_LOG("executor[%d] Destoried!", id_);
}

Result Executor::Execute() {
    RETURN_IF_ERR(init(), "Executor init fail");
    INFO_LOG("Executor [%d] load model start", id_);
    auto start_time = std::chrono::high_resolution_clock::now();

    RETURN_IF_ERR(loadModel(), "Exeuctor load model fail");

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    INFO_LOG("### Executor [%d] load model COST %f Second", id_, duration.count());

    Task task{};
    while (tq_->pop(task)) {
        INFO_LOG("### Executor[%d] popped task, queue size = %lu", id_, tq_->size());
        // 执行
        task.cb(run(std::move(task.inputs)));
    }
    RETURN_IF_ERR(unloadModel(), "Executor unload model fail");
    RETURN_IF_ERR(finalize(), "Executor finalize fail");
    return SUCCESS;
}

// 这里的初始化为在后端上初始化运行时资源
Result Executor::init() {
    INFO_LOG("Executor[%d] create stream start", id_);
    stream_ = backend_->createStream();
    if (stream_->getStream() == nullptr) {
        ERROR_LOG("executor get stream failed");
        return FAIL;
    }
    return SUCCESS;
}

Result Executor::finalize() {
    INFO_LOG("Executor[%d] destory stream start", id_);
    return backend_->destoryStream(stream_.get());
}

Result Executor::loadModel() {
    model_id_ = backend_->loadModel(model_path_);
    assert(model_id_ != -1);
    // info_ = backend_->getModelInfo(model_id_);
    // if (!info_) {
    //     ERROR_LOG("executor get model info failed");
    //     return FAIL;
    // }
    return SUCCESS;
}

Result Executor::unloadModel() {
    INFO_LOG("Executor[%d] unload model start", id_);
    assert(model_id_ != -1);
    return backend_->unloadModel(model_path_);
}

// 同步接口
std::vector<Tensor> Executor::run(std::vector<Tensor>&& inputs) {
    INFO_LOG("Executor[%d] Run", id_);
    return backend_->infer(stream_.get(), model_id_, std::move(inputs));
}