#pragma once
#include "common.h"
#include "backend/backend.h"
#include "backend/lynxi.h"
#include "backend/dummy.h"

class BackendFactory {
  public:
    static std::unique_ptr<Backend> createBackend(BackendType type) {
        switch (type) {
            case BACKEND_LYNXI: {
                int cnt;
                lynGetDeviceCount(&cnt);
                assert(cnt != 0);
                int device_id = 0; // hard code
                auto backend = std::make_unique<Lynxi>(device_id);
                auto res = backend->init();
                if (res == FAIL) {
                  return nullptr;
                }
                return backend;
            }
            case BACKEND_DUMMY:
                return std::make_unique<Dummy>();
            default:
                return nullptr;
        }
    }
};

class Prop {
  public:
    Prop(BackendType t, int id) : type(t), dev_id(id) {}
    ~Prop() = default;

    BackendType type;
    int dev_id;

    uint64_t memory_used{};
    uint64_t memory_total{};
    uint32_t usage_rate{}; // 计算单元使用率
    int32_t temperature_current{}; // 当前温度
    int32_t temperature_limit{}; // 停止温度

    void update();
};

class Monitor {
  public:
   
    static Monitor* getInstance() {
        // 单例模式
        static Monitor monitor;
        return &monitor;
    }

    Backend* getBackend(BackendType type);
    float getMemUsedRate(BackendType type);

  private:
    Monitor() {
      init();
    }
    ~Monitor() {
      stop_ = true;
      if (monitor_thread_.joinable()) {
        monitor_thread_.join();
      }
    }

    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;

    Result init();
    std::unordered_map<BackendType, std::unique_ptr<Backend>> backends_;
    std::unordered_map<BackendType, Prop> props_;
    std::thread monitor_thread_;
    std::atomic<bool> stop_;
};