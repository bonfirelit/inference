#include "common.h"
#include "session.h"
#include <yaml-cpp/yaml.h>

Session::Session(const std::string& yaml_file) {
    monitor_ = Monitor::getInstance();
    assert(monitor_ != nullptr);

    scfg_ = loadConfig(yaml_file);
    for (auto d : scfg_.devices) {
        if (d == "lynxi") {
            backends_.push_back(monitor_->getBackend(BACKEND_LYNXI));
        } else if (d == "ascend") {
            backends_.push_back(monitor_->getBackend(BACKEND_ACL));
        } else if (d == "dummy") {
            backends_.push_back(monitor_->getBackend(BACKEND_DUMMY));
        } else {
            assert(0);
        }
    }
    tq_ = std::make_unique<TaskQueue>();

    num_executor_ = scfg_.num_executor;
    num_task_ = scfg_.num_task;
    model_path_ = scfg_.model_path;
    executors_.reserve(num_executor_);
    for (int i = 0; i < num_executor_; i++) {
        // 暂时先都分配到第一个后端上
        executors_.emplace_back(std::make_unique<Executor>(model_path_, backends_[0], tq_.get(), i));
    }
}

/**  多后端
Session::Session(std::vector<BackendType>& types, int num_executor, const std::string& path,
const std::string& image_path) {
    monitor_ = Monitor::getInstance();
    tq_ = std::make_unique<TaskQueue>();

    for (auto type : types) {
        backedn1, backend2 = monitor_->getBackend(type);
        bacends_.push_back(backend1, backend2);
    }
    
    // 分配executor到不同的后端
    executors_.reserve(num_executor);
    for (int i = 0; i < n1; i++) {
        executors_.emplace_back(std::make_unique<Executor>(path, backend_1, tq_));
    }
    for (int i = 0; i < n2; i++) {
        executors_.emplace_back(std::make_unique<Executor>(path, backend_2, tq_));
    }
}
*/

void Session::preRun(int start, int end) {
    std::vector<std::string>& input_files = scfg_.input_files;

    for (int i = start; i <= end; i++) {
        std::vector<uint8_t> tensor_bytes;
        if (preprocess_fn_) {
            INFO_LOG("Session is preprocessing file[%d] now", i);
            auto start_time = std::chrono::high_resolution_clock::now();

            tensor_bytes = preprocess_fn_(input_files[i]);
            assert(tensor_bytes.size() > 0);

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;
            INFO_LOG("### Session preprocess file[%d] down, COST %f Second", i, duration.count());
        }

        // INFO_LOG("Session Create Task[%d] now", i);
        // TODO: 这里假设了模型只有一个输入张量
        std::vector<uint32_t> in_shape = scfg_.inputs[0].shape;
        auto in_dtype = scfg_.inputs[0].dtype;

        Task task{std::vector<Tensor>{{tensor_bytes, in_shape, stringToDataType(in_dtype)}}, 
                [this](std::vector<Tensor>&& outputs) {
                    outputs_.emplace_back(std::move(outputs));
                    // 每完成一个任务，计数器减一
                    if (task_counter_.fetch_sub(1) == 1) {
                        tq_->shutdown();  // 所有任务都处理完了
                    }
                }
            };
        tq_->push(task);
        INFO_LOG("### Produced task[%d], queue size = %lu", i, tq_->size());
    }
}


SessionOut Session::Run() {
    int n = scfg_.input_files.size();
    if (num_task_ != n) {
        WARN_LOG("num task %d don't equal to input file num %d", num_task_, n);
        num_task_ = std::min(num_task_, n);
    }
    task_counter_.store(num_task_);

    int num_preprocess_thread = 4;
    int chunk = (num_task_ + num_preprocess_thread - 1) / num_preprocess_thread;

    std::vector<std::thread> pre_threads;
    for (int t = 0; t < num_preprocess_thread; t++) {
        int start = t * chunk;
        int end   = std::min(start + chunk, num_task_);
        if (start >= end) break; // 没有任务可分配

        pre_threads.emplace_back([this, start, end]() {
            this->preRun(start, end);
        });
    }

    std::vector<std::thread> threads;
    threads.reserve(num_executor_);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_executor_; i++) {
        threads.emplace_back([this, i]() {
            auto start_time = std::chrono::high_resolution_clock::now();

            auto res = executors_[i]->Execute();

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;

            if (res != SUCCESS) {
                ERROR_LOG("Executor [%d] failed", i);
            } else {
                std::cout << "Executor [" << i << "] executed in " 
                      << duration.count() << " seconds." << std::endl;
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }
    for (auto& t : pre_threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_duration = end_time - start_time;

    // TODO:添加计算每个exeutor执行时间的代码
    // INFO_LOG("Session Run over, output size = %ld", outputs_[0].size());
    INFO_LOG("### Session使用%d个Executor, 耗时%f秒完成%d个任务的推理", num_executor_, total_duration.count(), num_task_);
    // 返回结果
    
    SessionOut ret;
    // INFO_LOG("-----session get output size = %d", (int)outputs_.size());
    ret.reserve(outputs_.size());

    // Transform tensors to vector of bytes
    for (auto result : outputs_) {
        // result是一个任务的所有输出张量
        std::vector<std::vector<uint8_t>> task_out;
        task_out.reserve(result.size());
        for (auto tensor : result) {
            task_out.emplace_back(tensor.asVector());
        }
        ret.push_back(task_out);
    }
    return ret;
    // 后处理？
}



SessionCfg Session::loadConfig(const std::string& yaml_file) {
    YAML::Node config = YAML::LoadFile(yaml_file);
    SessionCfg sc;

    sc.model_path   = config["model_path"].as<std::string>();
    sc.num_executor = config["num_executor"].as<int>();
    sc.num_task     = config["num_task"].as<int>();

    if (config["input_files"]) {
        YAML::Node in = config["input_files"];
        if (in.IsSequence()) {
            for (auto item : in) {
                std::string p = item.as<std::string>();
                auto files = filesFromPath(p);
                sc.input_files.insert(sc.input_files.end(), files.begin(), files.end());
            }
        } else if (in.IsScalar()) {
            std::string p = in.as<std::string>();
            auto files = filesFromPath(p);
            sc.input_files.insert(sc.input_files.end(), files.begin(), files.end());
        } else {
            std::cerr << "[Warning] input_files must be a string or a sequence of strings." << std::endl;
        }
    } else {
        std::cerr << "[Warning] no input_files defined in " << yaml_file << std::endl;
    }

    for (auto d : config["devices"]) {
        sc.devices.push_back(d.as<std::string>());
    }

    if (config["inputs"]) {
        for (auto item : config["inputs"]) {
            TensorCfg tc;
            if (item["shape"]) {
                for (auto dim : item["shape"])
                    tc.shape.push_back(dim.as<uint32_t>());
            }
            if (item["dtype"]) {
                tc.dtype = item["dtype"].as<std::string>();
            }
            sc.inputs.push_back(tc);
        }
    }

    if (config["outputs"]) {
        for (auto item : config["outputs"]) {
            TensorCfg tc;
            if (item["shape"]) {
                for (auto dim : item["shape"])
                    tc.shape.push_back(dim.as<uint32_t>());
            }
            if (item["dtype"]) {
                tc.dtype = item["dtype"].as<std::string>();
            }
            sc.outputs.push_back(tc);
        }
    }
    return sc;
}