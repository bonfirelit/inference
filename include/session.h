#pragma once

#include "common.h"
#include "executor.h"
#include "tensor.h"
#include "monitor.h"
#include "task_queue.h"

using PreprocessFn = std::function<std::vector<uint8_t>()>;
using PostprocessFn = std::function<void(const std::vector<Tensor>& outputs)>;


class Session {
  public:
    Session() = default;
    Session(BackendType type, int num_executor, const std::string& model_path,
    const std::string& image_path);
    ~Session() = default;

    std::vector<float> Run();

    void RegisterPreprocess(PreprocessFn fn) { preprocess_fn_ = std::move(fn); }
    void RegisterPostprocess(PostprocessFn fn) { postprocess_fn_ = std::move(fn); }

  private:
    int num_executor_;
    
    Monitor* monitor_;

    std::vector<std::unique_ptr<Executor>> executors_;
    std::unique_ptr<TaskQueue> tq_;
    std::string model_path_;
    // std::vector<Backend*> backends_;
    std::vector<Tensor> outputs_;
    std::atomic<int> task_counter_{0};
    PreprocessFn preprocess_fn_;
    PostprocessFn postprocess_fn_;

    // For Test
    Backend* backend_;
    std::string image_path_;
};