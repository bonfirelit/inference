#include "util.h"

size_t getElementSize(DataType dtype) {
    switch (dtype) {
    case DataType::FLOAT32: return 4;
    case DataType::INT8:   return 1;
    case DataType::FLOAT16: return 2;
    case DataType::UINT8: return 1;
    default: assert(0);
    }
}

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

// helper: 如果 p 是文件 -> 返回 {p}
//           如果 p 是目录 -> 返回 目录下所有普通文件（非递归）
//           如果不存在 -> 返回空 vector，并打印 warning
std::vector<std::string> filesFromPath(const std::string &p) {
    std::vector<std::string> files;
    std::error_code ec;
    std::filesystem::path path(p);

    if (!std::filesystem::exists(path, ec)) {
        std::cerr << "[Warning] path does not exist: " << p << std::endl;
        return files;
    }

    if (std::filesystem::is_regular_file(path, ec)) {
        files.push_back(path.string());
        return files;
    }

    if (std::filesystem::is_directory(path, ec)) {
        for (const auto &entry : std::filesystem::directory_iterator(path, ec)) {
            if (ec) {
                std::cerr << "[Warning] error iterating dir " << p << ": " 
                          << ec.message() << std::endl;
                break;
            }
            // 只收集普通文件
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
        // 保证目录内部文件顺序稳定
        std::sort(files.begin(), files.end());
        return files;
    }

    std::cerr << "[Warning] path is neither file nor directory: " << p << std::endl;
    return files;
}