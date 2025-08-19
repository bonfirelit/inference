#pragma once

#include "common.h"

class Executor;
class ModelInfo;

class Backend {
  public:
    virtual Result Init() = 0;
    virtual Result Finalize() = 0;
    virtual void Malloc(void **dev_ptr, size_t size) = 0;
    virtual void Free(void *dev_ptr) = 0;
    virtual Result MemCopy(void *dst, const void *src, uint64_t size, DIRECTION dir) = 0;
    virtual int LoadModel(const std::string &path) = 0;
    virtual Result UnloadModel(const std::string& path) = 0;
    virtual Result Infer(Executor* e, int model_id, void* dev_input_ptr, void* dev_output_ptr) = 0;
    virtual Result InitRtResource(Executor* e) = 0;
    virtual const ModelInfo* GetModelInfo(int model_id) const = 0;
    virtual Result FinalizeRtResource(Executor*e ) = 0;
};