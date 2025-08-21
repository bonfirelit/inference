#pragma once

#include "common.h"
#include "backend.h"
#include "executor.h"
#include "model_info.h"
#include "/usr/include/lynapi/lyn_api.h"


class Lynxi : public Backend {
  public:
    Lynxi(int dev_id);
    ~Lynxi();

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

    const BackendType getType() const { return type_; }

  private:
    BackendType type_{BACKEND_LYNXI};
    int device_id_;
    int next_model_id_{0};

    lynContext_t ctx_{};

    mutable std::mutex stream_lock_;
    mutable std::mutex model_lock_;

    std::unordered_map<Executor*, lynStream_t> executor2stream_;
    std::unordered_map<std::string, int> path2id_;
    std::unordered_map<int, lynModel_t> models_; // 后端上已加载的模型
    std::unordered_map<int, std::unique_ptr<ModelInfo>> infos_;
    
};