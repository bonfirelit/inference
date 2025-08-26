#pragma once


#include "backend.h"
#include "common.h"
#include "executor.h"
#include "model_info.h"

class Dummy : public Backend {
  public:
    Dummy() : Backend(BACKEND_DUMMY, 114514) {}
    virtual ~Dummy() {}

    Result init() override;
    Result finalize() override;
    Result malloc(void **dev_ptr, uint64_t size) override;
    Result free(void *dev_prt) override;
    Result memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) override;
    uint32_t loadModel(const std::string &path) override;
    Result unloadModel(const std::string& path) override;
    Result infer(Stream* stream, uint32_t model_id, void* dev_input_ptr, void* dev_output_ptr) override;
    std::unique_ptr<Stream> createStream() override;
    const ModelInfo* getModelInfo(uint32_t model_id) const override;
    Result destoryStream(Stream* stream) override;

  private:
    std::unique_ptr<ModelInfo> info_{nullptr};
};

class DummyStream : public Stream {
  public:
    DummyStream(Backend* backend) : Stream(backend) {}
    virtual ~DummyStream() {}
    Result createStream() override;
    Result destoryStream() override;
    void* getStream() override;

  private:
    void* stream_{nullptr};
};