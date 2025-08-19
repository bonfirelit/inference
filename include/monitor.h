#pragma once
#include "common.h"
#include "backend/backend.h"
#include "backend/lynxi.h"
#include "backend/dummy.h"

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
      Init();
      
    }
    ~Monitor() {
      stop_ = true;
      if (monitor_thread_.joinable()) {
        monitor_thread_.join();
      }
    }

    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;

    Result Init(); // 创建backend
    std::unordered_map<BackendType, std::unique_ptr<Backend>> backends_;
    std::unordered_map<BackendType, Prop> props_;
    std::thread monitor_thread_;
    std::atomic<bool> stop_;
};