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
    int model_id_{-1};
    
    const ModelInfo* info_{nullptr};
    
    void* dev_input_ptr_{nullptr};
    void* dev_output_ptr_{nullptr};
    
    Result LoadModel();
    Result UnloadModel();
    Result Init();
    Result Run();
    Result Finalize();
    Result PrepareInput(std::vector<Tensor>&&);
    Result PrepareOutput();
    void DestroyBuffers();
    std::vector<Tensor> GetOutput();
};