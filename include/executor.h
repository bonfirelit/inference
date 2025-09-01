#pragma once

#include "common.h"
#include "backend/backend.h"
#include "model_info.h"
#include "backend/lynxi.h"
#include "task_queue.h"
#include "tensor.h"
#include "util.h"

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
    /**
     * executor要执行的模型
     */
    uint32_t model_id_{0};
    
    const ModelInfo* info_{nullptr};
    std::unique_ptr<Stream> stream_;
    
    Result loadModel();
    Result unloadModel();
    Result init();
    std::vector<Tensor> run(std::vector<Tensor>&&);
    Result finalize();
};