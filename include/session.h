#pragma once

#include "common.h"
#include "executor.h"
#include "tensor.h"
#include "monitor.h"
#include "task_queue.h"
#include "util.h"
#include <any>

// 第一维是任务，第二维是每个任务的多个输出张量
using SessionOut = std::vector<std::vector<std::vector<uint8_t>>>;

using PreprocessFn = std::function<std::vector<uint8_t>(const std::any&)>;
using PostprocessFn = std::function<void(const std::vector<Tensor>& outputs)>;

struct TensorCfg {
    std::string name = "null";
    std::vector<uint32_t> shape;
    std::string dtype = "float32";
};


struct SessionCfg {
    std::string model_path;
    int num_executor;
    int num_task;
    int batch_size;
    std::vector<std::string> input_files;
    std::vector<std::string> devices;
    std::vector<TensorCfg> inputs;
    std::vector<TensorCfg> outputs;
};



class Session {
  public:
    Session() = default;
    Session(const std::string& yaml_file);
    ~Session() = default;

    SessionOut Run();

    void registerPreprocess(PreprocessFn fn) { preprocess_fn_ = std::move(fn); }
    void registerPostprocess(PostprocessFn fn) { postprocess_fn_ = std::move(fn); }

  private:
    SessionCfg loadConfig(const std::string& yaml_file);
    void preRun(int, int, int);
    void submitBatch(const std::vector<std::vector<uint8_t>>& batch_samples,
                          int padding_num);

    int num_executor_;
    int num_task_;
    
    Monitor* monitor_;

    std::vector<std::unique_ptr<Executor>> executors_;
    std::unique_ptr<TaskQueue> tq_;
    std::string model_path_;
    std::vector<Backend*> backends_;
    std::vector<std::vector<Tensor>> outputs_;
    std::atomic<int> task_counter_{0};
    SessionCfg scfg_;
    PreprocessFn preprocess_fn_;
    PostprocessFn postprocess_fn_;
};