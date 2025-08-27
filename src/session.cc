#include "common.h"
#include "session.h"
#include <yaml.h>

// 单后端
Session::Session(BackendType type, int num_executor, const std::string& model_path,
const std::string& image_path) {
    monitor_ = Monitor::getInstance();

    backend_ = monitor_->getBackend(type);
    assert(backend_ != nullptr);
    tq_ = std::make_unique<TaskQueue>();
    image_path_ = image_path;

    num_executor_ = num_executor;
    executors_.reserve(num_executor);
    for (int i = 0; i < num_executor; i++) {
        executors_.emplace_back(std::make_unique<Executor>(model_path, backend_, tq_.get(), i));
    }
}

Session::Session(const std::string& yaml_file) {
    monitor_ = Monitor::getInstance();

    scfg_ = loadConfig(yaml_file);
    for (auto d : scfg_.devices) {
        if (d == "lynxi") {
            // backend_ = monitor_->getBackend(BACKEND_LYNXI);
            backends_.push_back(monitor_->getBackend(BACKEND_LYNXI));
        } else if (d == "dummy") {
            // backend_ = monitor_->getBackend(BACKEND_DUMMY);
            backends_.push_back(monitor_->getBackend(BACKEND_DUMMY));
        } else {
            assert(0);
        }
    }
    tq_ = std::make_unique<TaskQueue>();
    // image_path_ = ;

    num_executor_ = scfg_.num_executor;
    num_task_ = scfg_.num_task;
    model_path_ = scfg_.model_path;
    executors_.reserve(num_executor_);
    for (int i = 0; i < num_executor_; i++) {
        // 暂时先都分配到第一个后端上
        executors_.emplace_back(std::make_unique<Executor>(model_path_, backends_[0], tq_.get(), i));
    }
}

//多后端
/** 
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


/*
std::vector<uint8_t> helper(const std::string& image_path) {
    // generate task. resnet50 input preprocess
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    assert(!image.empty());
    cv::Mat resized_image;
    // cv::resize(image, resized, cv::Size(224,224), 0, 0, cv::INTER_LINEAR);
    cv::resize(image, resized_image, cv::Size(256, 256));
    // 裁剪中心区域 224x224，从 (16,16) 开始
    cv::Rect roi(16, 16, 224, 224);
    cv::Mat cropped_image = resized_image(roi);
    // BGR -> RGB
    cv::Mat rgb_image;
    cv::cvtColor(cropped_image, rgb_image, cv::COLOR_BGR2RGB);
    // 转为 float32
    cv::Mat float_image;
    rgb_image.convertTo(float_image, CV_32FC3);
    // 按通道归一化： (val - mean) / std
    std::vector<float> mean = {123.0f, 117.0f, 104.0f};
    std::vector<float> std  = {57.0f,  57.0f,  58.0f};

    std::vector<cv::Mat> channels(3);
    cv::split(float_image, channels); // 分离 R, G, B 通道

    for (int i = 0; i < 3; ++i) {
        channels[i] = (channels[i] - mean[i]) / std[i];
    }

    cv::Mat normalized_image;
    cv::merge(channels, normalized_image); // 仍为 HWC，float32

    // 7. HWC -> CHW，展开成 1x3x224x224
    std::vector<cv::Mat> chw_channels(3);
    cv::split(normalized_image, chw_channels); // 每个通道是 224x224

    // 创建 1 x (3x224x224) 的 float32 Mat
    cv::Mat chw_tensor(1, 3 * 224 * 224, CV_32F);

    for (int i = 0; i < 3; ++i) {
        std::memcpy(
            chw_tensor.ptr<float>() + i * 224 * 224,
            chw_channels[i].ptr<float>(),
            224 * 224 * sizeof(float)
        );
    }
    
    // 获取数据指针和总字节数
    float* float_ptr = chw_tensor.ptr<float>();
    size_t float_count = 3 * 224 * 224;
    size_t byte_count = float_count * sizeof(float); // 每个 float 是 4 字节

    // 构造 std::vector<uint8_t>，使用 reinterpret_cast
    std::vector<uint8_t> tensor_bytes(
        reinterpret_cast<uint8_t*>(float_ptr),
        reinterpret_cast<uint8_t*>(float_ptr) + byte_count
    );

    return tensor_bytes;
}

*/

SessionOut Session::Run() {
    assert(monitor_ != nullptr);

    for (int i = 0; i < num_task_; i++) {
        std::vector<uint8_t> tensor_bytes;
        if (preprocess_fn_) {
            INFO_LOG("Session is preprocessing now");
            tensor_bytes = preprocess_fn_("bad_path");
            INFO_LOG("Session preprocess down");
        }

        INFO_LOG("Session Create Task[%d] now", i);
        std::vector<uint32_t> in_shape = scfg_.inputs[0].shape;
        // INFO_LOG("The input shape is:");
        // for (auto x : in_shape) {
        //     printf("%d ", x);
        // }
        // printf("\n");
        Task task{std::vector<Tensor>{{tensor_bytes, in_shape, FLOAT32}}, 
                [this](std::vector<Tensor>&& outputs) {
                    outputs_.emplace_back(std::move(outputs));
                    // 每完成一个任务，计数器减一
                    if (task_counter_.fetch_sub(1) == 1) {
                        tq_->shutdown();  // 所有任务都处理完了
                    }
                }
            };
        task_counter_.fetch_add(1);
        tq_->push(task);
    }
    

    std::vector<std::thread> threads;
    threads.reserve(num_executor_);

    for (int i = 0; i < num_executor_; i++) {
        threads.emplace_back([this, i]() {
            auto res = executors_[i]->Execute();
            if (res != SUCCESS) {
                ERROR_LOG("Executor [%d] failed", i);
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    // TODO:添加计算每个exeutor执行时间的代码
    INFO_LOG("Session Run over, output size = %ld", outputs_.size());
    // 返回结果
    
    SessionOut ret;
    ret.reserve(outputs_.size());

    for (auto result : outputs_) {
        // result是一个任务的所有输出张量
        std::vector<std::vector<float>> task_out;
        task_out.reserve(result.size());
        for (auto tensor : result) {
            task_out.emplace_back(tensor.asVector<float>());
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