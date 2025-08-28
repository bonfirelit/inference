#pragma once 

#include <cstdint>
#include <cassert>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>
#include <memory>
#include <numeric>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdint>
#include <string>

// INFO_LOG("info %d %d", 8, num);
#define INFO_LOG(fmt, ...) fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) fprintf(stdout, "[WARN]  " fmt "\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) fprintf(stderr, "[ERROR]  " fmt "\n", ##__VA_ARGS__)


enum DataType {
  FLOAT32,
  INT8,
  UINT8,
  FLOAT16,
};

enum Result {
    FAIL,
    SUCCESS,
};

enum BackendType {
    BACKEND_UNKNOWN,
    BACKEND_DUMMY,
    BACKEND_LYNXI,
    BAKCEND_CPU,
};

enum DIRECTION {
    HOST2DEVICE,
    DEVICE2HOST,
    HOST2HOST,
};

#define RETURN_IF_ERR(expr, msg)                           \
  do {                                                     \
    auto err__ = (expr);                                   \
    if (err__ != SUCCESS) {                                \
      std::cerr << "[ERROR] " << #expr << " failed: " << msg << std::endl; \
      return err__;                                        \
    }                                                      \
  } while (0)


// #define LYNXI_CHECK_ERR(X, fmt, ...)           \
//   do {                               \
//     lynError_t err__ = (X); \
//     if (err__ != 0) {          \
//       fprintf(stderr, "[ERROR]  " fmt "\n", ##__VA_ARGS__); \
//       return Result::FAIL;                  \
//     }                                \
//   } while (0)


static DataType stringToDataType(const std::string& str) {
    std::string s = str;

    if (s == "float32") return FLOAT32;
    else if (s == "int8") return INT8;
    else if (s == "uint8") return UINT8;
    else if (s == "float16") return FLOAT16;
    else assert(0);
}
