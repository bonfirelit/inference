#pragma once

#include "common.h"
#include "backend.h"
#include "executor.h"
#include "model_info.h"
#include <lyn_api.h>
#include <lyn_smi.h>

class Lynxi : public Backend {
  public:
    Lynxi(int dev_id);
    virtual ~Lynxi();

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
    lynContext_t ctx_{nullptr};

};

class LynxiModel : public Model {
  public:
    LynxiModel(Backend* backend, lynModel_t model) : Model(backend), model_(model) {}
    virtual ~LynxiModel() {}
    void* getHandle() override;
  private:
    lynModel_t model_;
};

class LynxiStream : public Stream {
  public:
    LynxiStream(Backend* backend) : Stream(backend) {}
    virtual ~LynxiStream() {}
    Result synchronize() override;
    Result createStream() override;
    Result destoryStream() override;
    Result recordEvent(Event* event) override;
    Result waitEvent(Event* event) override;
    void* getStream() override;

  private:
    lynStream_t stream_{nullptr};
};

class LynxiEvent : public Event {
  public:
    LynxiEvent(Backend* backend) : Event(backend) {}
    virtual ~LynxiEvent() {}
    Result synchronize() override;
    Result createEvent() override;
    Result destoryEvent() override;
    void* getEvent() override;

  private:
    lynEvent_t event_{nullptr};
};