#pragma once

#include "common.h"
#include "backend.h"
#include "model_info.h"
#include "tensor.h"

class Ascend : public Backend {
  public:
    Ascend(int dev_id);
    virtual ~Ascend();

    Result init() override;
    Result finalize() override;
    Result malloc(void **dev_ptr, uint64_t size) override;
    Result free(void *dev_prt) override;
    Result memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) override;
    uint32_t loadModel(const std::string &path) override;
    Result unloadModel(const std::string& path) override;
    Result infer(Stream* stream, uint32_t model_id, void* dev_input_ptr, void* dev_output_ptr) override;
    const ModelInfo* getModelInfo(uint32_t model_id) const override;
    
    std::unique_ptr<Stream> createStream() override;
    Result destoryStream(Stream* stream) override;

  private:
    aclrtContext ctx_{nullptr};

};

class AscendModel : public Model {
  public:
    AscendModel(Backend* backend, uint32_t model) : Model(backend), model_(model) {}
    virtual ~AscendModel() {}
    void* getHandle() override;
  private:
    uint32_t model_;
};

class AscendStream : public Stream {
  public:
    AscendStream(Backend* backend) : Stream(backend) {}
    virtual ~AscendStream() {}
    Result synchronize() override;
    Result createStream() override;
    Result destoryStream() override;
    Result recordEvent(Event* event) override;
    Result waitEvent(Event* event) override;
    void* getStream() override;

  private:
    aclrtStream stream_{nullptr};
};

class AscendEvent : public Event {
  public:
    AscendEvent(Backend* backend) : Event(backend) {}
    virtual ~AscendEvent() {}
    Result synchronize() override;
    Result createEvent() override;
    Result destoryEvent() override;
    void* getEvent() override;

  private:
    aclrtEvent event_{nullptr};
};