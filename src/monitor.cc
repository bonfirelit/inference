#include "monitor.h"

Result Monitor::Init() {
    // 创建backend
    // lynxi，假设只有一块设备
    int cnt;
    lynGetDeviceCount(&cnt);
    assert(cnt != 0);
    int device_id = 0;
    backends_.emplace(BACKEND_LYNXI, std::make_unique<Lynxi>(device_id));
    // 调用lynxi的初始化函数(TODO:感觉应该把Init放到backend的构造函数里)
    backends_[BACKEND_LYNXI]->init();
    props_.emplace(BACKEND_LYNXI, Prop{BACKEND_LYNXI, device_id});

    backends_.emplace(BACKEND_DUMMY, std::make_unique<Dummy>());

    // 开启监控线程
    // monitor_thread_ = std::thread([this]() {
    //     while (!stop_) {
    //         for (auto& [type, prop] : props_) {
    //             prop.update();
    //         }
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //     }
    // });

    return SUCCESS;
}

Backend* Monitor::getBackend(BackendType type) {
    // 可以做成懒加载。在这里如果没有找到对应type的后端，可以尝试创建并返回
    // 相应的，Monitor的初始化函数中就可以不用创建后端了。使用工厂模式加载
    if (backends_.find(type) == backends_.end()) {
        INFO_LOG("后端不存在");
        return nullptr;
    }
    return backends_[type].get();
}

float Monitor::getMemUsedRate(BackendType type) {
    auto it = props_.find(type);
    if (it == props_.end()) {
        ERROR_LOG("prop不存在");
        return 0.0;
    }
    return (float)(it->second.memory_used) / (float)(it->second.memory_total);
}

void Prop::update() {
    if (type == BACKEND_LYNXI) {
        lynDeviceProperties_t device_prop;
        auto err = lynGetDeviceProperties(dev_id, &device_prop);
        if (err != 0) {
            ERROR_LOG("lynxi获取设备属性失败");
            return;
        }
        memory_used = device_prop.deviceMemoryUsed;
        memory_total = device_prop.deviceMemoryTotal;
        usage_rate = device_prop.deviceApuUsageRate;
        temperature_current = device_prop.deviceTemperatureCurrent;
        temperature_limit = device_prop.deviceTemperatureLimit;
    }
}