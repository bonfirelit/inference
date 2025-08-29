#include "monitor.h"

Result Monitor::init() {
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
    auto it = backends_.find(type);
    if (it == backends_.end()) {
        auto backend = BackendFactory::createBackend(type);
        if (!backend) {
            ERROR_LOG("can not create backend");
            return nullptr;
        }
        backends_[type] = std::move(backend);
        props_.emplace(type, Prop{type, 0}); // warn: assume only one device and id is zero
        return backends_[type].get();
    }
    return it->second.get();
}

float Monitor::getMemUsedRate(BackendType type) {
    auto it = props_.find(type);
    if (it == props_.end()) {
        ERROR_LOG("prop doesn't exist");
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