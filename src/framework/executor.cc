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
    info_ = backend_->getModelInfo(model_id_);
    if (!info_) {
        ERROR_LOG("executor get model info failed");
        return FAIL;
    }
    return SUCCESS;
}

Result Executor::unloadModel() {
    INFO_LOG("Executor[%d] unload model start", id_);
    assert(model_id_ != -1);
    return backend_->unloadModel(model_path_);
}

// 在设备上分配内存并转移数据
/*
Result Executor::prepareInput(std::vector<Tensor>&& inputs) {
    assert(info_ != nullptr);

    size_t inum = inputs.size(), mnum = info_->getInputNum();
    if (inum != mnum) {
        ERROR_LOG("输入张量个数[%zu]不匹配模型要求输入张量个数[%zu]",
                    inum, mnum);
        return FAIL;
    }

    size_t input_size = 0;
    for (auto& tensor : inputs) {
        input_size += tensor.size();
    }
    size_t model_input_size = info_->getBatchSize() * info_->getInputSize();
    if (input_size != model_input_size) {
        ERROR_LOG("input size[%zu] does not match model input size[%zu]", 
                    input_size, model_input_size);
        return FAIL;
    }

    backend_->malloc((void**)&dev_input_ptr_, input_size);
    assert(dev_input_ptr_ != nullptr);
    auto temp = static_cast<char*>(dev_input_ptr_);
    for (auto& tensor : inputs) {
        auto size = tensor.size();
        backend_->memcopy(temp, tensor.data(), size, HOST2DEVICE);
        temp += size;
    }
    return SUCCESS;
}
*/

/*
Result Executor::prepareOutput() {
    size_t model_output_size = info_->getBatchSize() * info_->getOutputSize();
    backend_->malloc((void**)&dev_output_ptr_, model_output_size);
    assert(dev_output_ptr_ != nullptr);
    return SUCCESS;
}
*/

// 同步接口
std::vector<Tensor> Executor::run(std::vector<Tensor>&& inputs) {
    INFO_LOG("Executor[%d] Run", id_);
    return backend_->infer(stream_.get(), model_id_, std::move(inputs));
}

// 将输出数据搬回主机
// std::vector<Tensor> Executor::getOutput() {
//     auto output_num = info_->getOutputNum();
//     std::vector<Tensor> outputs;
//     outputs.reserve(output_num);

//     auto shapes = info_->getOutputsShape();
//     assert(shapes.size() == output_num);

//     auto dev_out_ptr = static_cast<const char*>(dev_output_ptr_);
//     for (int i = 0; i < output_num; i++) {
//         INFO_LOG("Executor is get number %d output tensor", i);
//         outputs.emplace_back(shapes[i], output_type_);
//         Tensor& tensor = outputs.back();
//         size_t output_size = getElementSize(output_type_) * std::accumulate(
//           shapes[i].begin(), shapes[i].end(), 1u, std::multiplies<uint32_t>()
//           );
//         assert(tensor.size() == output_size);
//         assert(tensor.data() != nullptr);
//         auto err = backend_->memcopy(
//             tensor.data(),
//             dev_out_ptr + i * output_size,
//             output_size,
//             DEVICE2HOST
//         );
//         if (err != SUCCESS) {
//             ERROR_LOG("Executor Getoutput fail");
//             return {};
//         }
//     }
//     return outputs;
// }

// void Executor::destroyBuffers() {
//     backend_->free(dev_input_ptr_);
//     backend_->free(dev_output_ptr_);
// }
