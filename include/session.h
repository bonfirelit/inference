#pragma once

#include "common.h"
#include "executor.h"
#include "tensor.h"
#include "monitor.h"
#include "task_queue.h"

using PreprocessFn = std::function<std::vector<uint8_t>()>;
using PostprocessFn = std::function<void(const std::vector<Tensor>& outputs)>;

struct TensorCfg {
    std::string name = "null";
    std::vector<uint32_t> shape;
    std::string dtype = "float32";
};


struct SessionCfg {
    std::string model_path;
    int num_executor;
    std::vector<std::string> devices;
    std::vector<TensorCfg> inputs;
    std::vector<TensorCfg> outputs;
};



class Session {
  public:
    Session() = default;
    Session(BackendType type, int num_executor, const std::string& model_path,
    const std::string& image_path);
    Session(const std::string& yaml_file);
    ~Session() = default;

    std::vector<float> Run();

    void RegisterPreprocess(PreprocessFn fn) { preprocess_fn_ = std::move(fn); }
    void RegisterPostprocess(PostprocessFn fn) { postprocess_fn_ = std::move(fn); }

  private:
    SessionCfg loadConfig(const std::string& yaml_file);
    int num_executor_;
    
    Monitor* monitor_;

    std::vector<std::unique_ptr<Executor>> executors_;
    std::unique_ptr<TaskQueue> tq_;
    std::string model_path_;
    // std::vector<Backend*> backends_;
    std::vector<Tensor> outputs_;
    std::atomic<int> task_counter_{0};
    SessionCfg scfg_;
    PreprocessFn preprocess_fn_;
    PostprocessFn postprocess_fn_;

    // For Test
    Backend* backend_;
    std::string image_path_;
};