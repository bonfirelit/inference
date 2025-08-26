#pragma once

#include "common.h"
#include "backend/backend.h"
#include "model_info.h"
#include "backend/lynxi.h"
#include "task_queue.h"
#include "tensor.h"

class Executor {
  public:
    Executor(const std::string& model_path, Backend* backend, TaskQueue* tq, int id);
    ~Executor();
    Result Execute();
    
    private:
    int id_;
    Backend* backend_;
    std::string model_path_;
    TaskQueue* tq_;
    uint32_t model_id_{0};
    
    const ModelInfo* info_{nullptr};
    std::unique_ptr<Stream> stream_;
    
    void* dev_input_ptr_{nullptr};
    void* dev_output_ptr_{nullptr};
    
    Result loadModel();
    Result unloadModel();
    Result init();
    Result run();
    Result finalize();
    Result prepareInput(std::vector<Tensor>&&);
    Result prepareOutput();
    void destroyBuffers();
    std::vector<Tensor> getOutput();
};