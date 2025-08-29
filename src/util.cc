#include "util.h"

DataType stringToDataType(const std::string& str) {
    std::string s = str;

    if (s == "float32") return FLOAT32;
    else if (s == "int8") return INT8;
    else if (s == "uint8") return UINT8;
    else if (s == "float16") return FLOAT16;
    else assert(0);
}

std::vector<uint16_t> bytesToUint16(const std::vector<uint8_t>& bytes) {
    assert(bytes.size() % 2 == 0);

    size_t count = bytes.size() / 2;
    std::vector<uint16_t> result(count);

    for (size_t i = 0; i < count; ++i) {
        result[i] = static_cast<uint16_t>(bytes[2 * i]) |
                    (static_cast<uint16_t>(bytes[2 * i + 1]) << 8);
    }

    return result;
}

std::vector<float> bytesToFloat32(const std::vector<uint8_t>& bytes) {
    assert(bytes.size() % 4 == 0);

    size_t count = bytes.size() / 4;
    std::vector<float> result(count);

    std::memcpy(result.data(), bytes.data(), bytes.size());

    return result;
}

std::vector<int> top5Indices(const std::vector<float>& res) {
    // 构造索引数组 [0, 1, 2, ..., N-1]
    std::vector<int> indices(res.size());
    for (int i = 0; i < (int)res.size(); ++i) {
        indices[i] = i;
    }

    // 只排序前 5 个
    std::partial_sort(
        indices.begin(), indices.begin() + 5, indices.end(),
        [&](int a, int b) {
            return res[a] > res[b];  // 按值从大到小
        }
    );

    return std::vector<int>(indices.begin(), indices.begin() + 5);
}