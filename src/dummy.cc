#include "backend/dummy.h"
#include <cstring>

Result Dummy::init() {
    INFO_LOG("Dummy Init Success");
    return SUCCESS;
}

Result Dummy::finalize() {
    INFO_LOG("Dummy Finalize Success");
    return SUCCESS;
}

Result Dummy::malloc(void **dev_ptr, uint64_t size) {
    INFO_LOG("--Dummy Malloc Start--");
    *dev_ptr = std::malloc(size);
    INFO_LOG("Dummy malloc %lu bytes at addr %p", size, *dev_ptr);
    return SUCCESS;
}

Result Dummy::free(void *dev_ptr) {
    INFO_LOG("--Dummy free start--");
    std::free(dev_ptr);
    INFO_LOG("Dummy free mem at addr %p", dev_ptr);
    return SUCCESS;
}

Result Dummy::memcopy(void *dst, const void *src, uint64_t size, DIRECTION dir) {
    INFO_LOG("--Dummy Memcpy Start--");
    assert(dir == HOST2HOST);
    std::memcpy(dst, src, size);
    INFO_LOG("Dummy copied %lu bytes mem form %p to %p", size, src, dst);
    return SUCCESS;
}

uint32_t Dummy::loadModel(const std::string &path) {
    INFO_LOG("--Dummy loadModel Start--");
    uint32_t id = 114514;
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (info_ != nullptr) {
            return id;
        }
    }
    size_t batch = 1, output_size = 20, input_size = 20, input_num = 1, output_num = 1;
    std::vector<std::vector<uint32_t>> ins_dim{{1, 5}};
    std::vector<std::vector<uint32_t>> outs_dim{{1, 5}};
    {
        std::lock_guard<std::mutex> lock(model_lock_);
        if (info_ != nullptr) {
            return id;
        }
        info_ = std::make_unique<ModelInfo>(batch, input_size, output_size, input_num, output_num, 
            std::move(ins_dim), std::move(outs_dim));
    }
    INFO_LOG("Dummy load model success, id = %d", id);
    return id;
}

Result Dummy::unloadModel(const std::string& path) {
    INFO_LOG("--Dummy UnloadModel Start--");
    INFO_LOG("Dummy unload model success");
    return SUCCESS;
}

Result Dummy::infer(Stream* stream, uint32_t model_id, void* dev_input_ptr, void* dev_output_ptr) {
    // std::lock_guard<std::mutex> lock(model_lock_);
    // if (!info_) {
    //     ERROR_LOG("Dummy infer failed: no model loaded");
    //     return FAIL;
    // }
    INFO_LOG("--Dummy infer Start--");
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

std::unique_ptr<Stream> Dummy::createStream() {
    INFO_LOG("--Dummy createStream Start--");
    std::unique_ptr<DummyStream> dummy_stream = std::make_unique<DummyStream>(this);
    dummy_stream->createStream();
    return dummy_stream;
}

const ModelInfo* Dummy::getModelInfo(uint32_t model_id) const {
    std::lock_guard<std::mutex> lock(model_lock_);
    INFO_LOG("--Dummy getModelInfo Start--");
    INFO_LOG("Dummy get %d model info", model_id);
    return info_.get();
}

Result Dummy::destoryStream(Stream* stream) {
    INFO_LOG("--Dummy destoryStream Start--");
    stream->destoryStream();
    INFO_LOG("Dummy destoryStream success");
    return SUCCESS;
}

Result DummyStream::createStream() {
    INFO_LOG("#### DummyStream create stream success ####");
    stream_ = std::malloc(1);
    assert(stream_ != nullptr);
    return SUCCESS;
}

Result DummyStream::destoryStream() {
    INFO_LOG("#### DummyStream destory stream success ####");
    std::free(stream_);
    return SUCCESS;
}

void* DummyStream::getStream() {
    return stream_;
}
