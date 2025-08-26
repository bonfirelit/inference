#pragma once

#include "common.h"

class Executor;
class ModelInfo;
class Stream;
class Model;
class Event;

class Backend {
  public:
    Backend(BackendType type, int device_id) 
    : type_(type),
      device_id_(device_id) {}    
    virtual ~Backend() = default;

    virtual Result init() = 0;
    virtual Result finalize() = 0;
    virtual Result malloc(void **dev_ptr, size_t size) = 0;
    virtual Result free(void *dev_ptr) = 0;
    virtual Result memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) = 0;
    virtual uint32_t loadModel(const std::string &path) = 0;
    virtual Result unloadModel(const std::string& path) = 0;
    virtual Result infer(Stream* stream, uint32_t model_id, void* dev_input_ptr, void* dev_output_ptr) = 0;
    virtual const ModelInfo* getModelInfo(uint32_t model_id) const = 0;

    virtual std::unique_ptr<Stream> createStream() = 0;
    virtual Result destoryStream(Stream* stream) = 0;

    BackendType getBackendType() { return type_; }

  protected:
    int device_id_;
    uint32_t next_model_id_{0};
    BackendType type_;

    mutable std::mutex model_lock_;

    std::unordered_map<std::string, uint32_t> path_to_id_;
    std::unordered_map<uint32_t, std::unique_ptr<Model>> models_;
    std::unordered_map<uint32_t, std::unique_ptr<ModelInfo>> infos_;
};

class Model {
  public:
    Model(Backend* backend) : backend_(backend) {}
    virtual ~Model() {}
    virtual void* getHandle() = 0;
    Backend* getBackend() { return backend_; }

  protected:
    Backend* backend_;
};

class Stream {
  public:
    Stream() = default;
    Stream(Backend* backend) : backend_(backend) {}
    virtual ~Stream() {}
    virtual Result synchronize() { return SUCCESS; }
    virtual Result createStream() { return SUCCESS; }
    virtual Result destoryStream() { return SUCCESS; }
    virtual Result recordEvent(Event* event) { return SUCCESS; }
    virtual Result waitEvent(Event* event) { return SUCCESS; }
    virtual void* getStream() { return nullptr; }
    Backend* getBackend() { return backend_; }
    
  protected:
    Backend* backend_; // stream所属的backend
};

class Event {
  public:
    Event(Backend* backend) : backend_(backend) {}
    virtual ~Event() {}
    virtual Result synchronize() { return SUCCESS; }
    virtual Result createEvent() { return SUCCESS; }
    virtual Result destoryEvent() { return SUCCESS; }
    virtual void* getEvent() { return nullptr; }
    Backend* getBackend() { return backend_; }

  protected:
    Backend* backend_;
};