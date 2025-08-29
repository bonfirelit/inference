#pragma once
#include "common.h"

DataType stringToDataType(const std::string& str);
std::vector<uint16_t> bytesToUint16(const std::vector<uint8_t>& bytes);
std::vector<float> bytesToFloat32(const std::vector<uint8_t>& bytes);
std::vector<int> top5Indices(const std::vector<float>& res);

template <typename T>
void printVector(const std::vector<T>& v) {
    std::cout << "[ ";
    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << "]" << std::endl;
}