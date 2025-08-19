#include "backend/dummy.h"
#include <cstring>

Result Dummy::Init() {
    INFO_LOG("Dummy Init Success");
    return SUCCESS;
}

Result Dummy::Finalize() {
    INFO_LOG("Dummy Finalize Success");
    return SUCCESS;
}

void Dummy::Malloc(void **dev_ptr, uint64_t size) {
    INFO_LOG("--Dummy Malloc Start--");
    *dev_ptr = malloc(size);
    INFO_LOG("Dummy malloc %lu bytes at addr %p", size, *dev_ptr);
}

void Dummy::Free(void *dev_ptr) {
    INFO_LOG("--Dummy Free start--");
    free(dev_ptr);
    INFO_LOG("Dummy free mem at addr %p", dev_ptr);
}

Result Dummy::MemCopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
    INFO_LOG("--Dummy Memcpy Start--");
    assert(dir == HOST2HOST);
    std::memcpy(dst, src, size);
    INFO_LOG("Dummy copied %lu bytes mem form %p to %p", size, src, dst);
    return SUCCESS;
}

int Dummy::LoadModel(const std::string &path) {
    INFO_LOG("--Dummy LoadModel Start--");
    size_t batch = 1, output_size = 20, input_size = 20, input_num = 1, output_num = 1;
    std::vector<std::vector<uint32_t>> ins_dim{{1, 5}};
    std::vector<std::vector<uint32_t>> outs_dim{{1, 5}};
    info_ = std::make_unique<ModelInfo>(batch, input_size, output_size, input_num, output_num, 
        std::move(ins_dim), std::move(outs_dim));
    int id = 114514;
    INFO_LOG("Dummy load model success, id = %d", id);
    return id;
}

Result Dummy::UnloadModel(const std::string& path) {
    INFO_LOG("--Dummy UnloadModel Start--");
    INFO_LOG("Dummy unload model success");
    return SUCCESS;
}

Result Dummy::Infer(Executor* e, int model_id, void* dev_input_ptr, void* dev_output_ptr) {
    INFO_LOG("--Dummy Infer Start--");
    float* input = static_cast<float*>(dev_input_ptr);
    float* output = static_cast<float*>(dev_output_ptr);
    int len = 5;
    float cons = 1.0;
    for (int i = 0; i < len; i++) {
        output[i] = input[i] + cons;
    }
    INFO_LOG("Dummy infer success");
    return SUCCESS;
}

Result Dummy::InitRtResource(Executor* e) {
    INFO_LOG("--Dummy InitRtResource Start--");
    INFO_LOG("Dummy init rt resource success");
    return SUCCESS;
}

const ModelInfo* Dummy::GetModelInfo(int model_id) const {
    INFO_LOG("--Dummy GetModelInfo Start--");
    INFO_LOG("Dummy get %d model info", model_id);
    return info_.get();
}

Result Dummy::FinalizeRtResource(Executor*e ) {
    INFO_LOG("--Dummy FinalizeRtResource Start--");
    INFO_LOG("Dummy finalize rt resource success");
    return SUCCESS;
}


