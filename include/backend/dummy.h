#pragma once


#include "backend.h"
#include "common.h"
#include "executor.h"
#include "model_info.h"

class Dummy : public Backend {
  public:
    Dummy() = default;
    ~Dummy() = default;

    Result Init() override;
    Result Finalize() override;
    void Malloc(void **dev_ptr, uint64_t size) override;
    void Free(void *dev_prt) override;
    Result MemCopy(void *dst, const void *src, uint64_t size, DIRECTION dir) override;
    int LoadModel(const std::string &path) override;
    Result UnloadModel(const std::string& path) override;
    Result Infer(Executor* e, int model_id, void* dev_input_ptr, void* dev_output_ptr) override;
    Result InitRtResource(Executor* e) override;
    const ModelInfo* GetModelInfo(int model_id) const override;
    Result FinalizeRtResource(Executor*e ) override;

  private:
    BackendType type_{BACKEND_DUMMY};
    std::unique_ptr<ModelInfo> info_;
};