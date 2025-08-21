#include "executor.h"

// 要保证调用构造函数时，backend_type一定是合法的
// 入参的backend指针代表该执行器在这个后端上运行
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
    INFO_LOG("Executor[%d] Init", id_);
    RETURN_IF_ERR(Init(), "Executor init fail");
    INFO_LOG("Executor[%d] LoadModel", id_);
    RETURN_IF_ERR(LoadModel(), "Exeuctor load model fail");
    Task task{};
    while (tq_->pop(task)) {
        // 分配设备内存
        INFO_LOG("Executor[%d] PrepareInput", id_);
        RETURN_IF_ERR(PrepareInput(std::move(task.inputs)), "Executor failed to prepare input");
        INFO_LOG("Executor[%d] PrepareOutput", id_);
        RETURN_IF_ERR(PrepareOutput(), "Executor fail to prepare output");
        // 执行
        INFO_LOG("Executor[%d] Run", id_);
        RETURN_IF_ERR(Run(), "Executor run fail");
        // 回调函数将结果传输至session
        INFO_LOG("Executor[%d] GetOutput", id_);
        task.cb(GetOutput());
        // 释放设备内存
        DestroyBuffers();
    }
    RETURN_IF_ERR(UnloadModel(), "Executor unload model fail");
    RETURN_IF_ERR(Finalize(), "Executor finalize fail");
    return SUCCESS;
}

// 这里的初始化为在后端上初始化运行时资源
Result Executor::Init() {
    return backend_->InitRtResource(this);
}

Result Executor::Finalize() {
    return backend_->FinalizeRtResource(this);
}

Result Executor::LoadModel() {
    model_id_ = backend_->LoadModel(model_path_);
    assert(model_id_ != -1);
    info_ = backend_->GetModelInfo(model_id_);
    if (!info_) {
        ERROR_LOG("执行器获取ModelInfo失败");
        return FAIL;
    }
    return SUCCESS;
}

Result Executor::UnloadModel() {
    assert(model_id_ != -1);
    return backend_->UnloadModel(model_path_);
}

// 在设备上分配内存并转移数据
Result Executor::PrepareInput(std::vector<Tensor>&& inputs) {
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

    backend_->Malloc((void**)&dev_input_ptr_, input_size);
    assert(dev_input_ptr_ != nullptr);
    auto temp = static_cast<char*>(dev_input_ptr_);
    for (auto& tensor : inputs) {
        auto size = tensor.size();
        backend_->MemCopy(temp, tensor.data(), size, HOST2HOST);
        // backend_->MemCopy(temp, tensor.data(), size, HOST2DEVICE);
        temp += size;
    }
    return SUCCESS;
}

Result Executor::PrepareOutput() {
    size_t model_output_size = info_->getBatchSize() * info_->getOutputSize();
    backend_->Malloc((void**)&dev_output_ptr_, model_output_size);
    assert(dev_output_ptr_ != nullptr);
    return SUCCESS;
}

// 同步接口
Result Executor::Run() {
    return backend_->Infer(this, model_id_, dev_input_ptr_, dev_output_ptr_);
}

// 将输出数据搬回主机
std::vector<Tensor> Executor::GetOutput() {
    auto output_num = info_->getOutputNum();
    auto output_size = info_->getOutputSize();
    std::vector<Tensor> outputs;
    outputs.reserve(output_num);

    auto shapes = info_->getOutputsShape();
    assert(shapes.size() == output_num);

    auto dev_out_ptr = static_cast<const char*>(dev_output_ptr_);
    for (int i = 0; i < output_num; i++) {
        outputs.emplace_back(shapes[i], FLOAT32);
        Tensor& tensor = outputs.back();
        assert(tensor.size() == output_size);
        assert(tensor.data() != nullptr);
        // auto err = backend_->MemCopy(
        //     tensor.data(),
        //     dev_out_ptr + i * output_size,
        //     output_size,
        //     DEVICE2HOST
        // );
        auto err = backend_->MemCopy(
            tensor.data(),
            dev_out_ptr + i * output_size,
            output_size,
            HOST2HOST
        );
        if (err != SUCCESS) {
            ERROR_LOG("Executor Getoutput fail");
            return {};
        }
    }
    return outputs;
}

void Executor::DestroyBuffers() {
    backend_->Free(dev_input_ptr_);
    backend_->Free(dev_output_ptr_);
}
