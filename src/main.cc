#include "session.h"
#include <fstream>
#include <cstring>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

void printVector(std::vector<uint16_t> v) {
    std::cout << "[ ";
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << "]" << std::endl;
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

/*
std::vector<uint8_t> resnetPreprocess(const std::any& arg) {
    auto path = std::any_cast<std::string>(&arg);
    if (path == nullptr) {
        return {};
    }
    cv::Mat img = cv::imread(*path, cv::IMREAD_COLOR);
    assert(!img.empty());
    // 转换为 RGB
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    // 缩放短边到 256
    int h = img.rows, w = img.cols;
    int new_h, new_w;
    if (h < w) {
        new_h = 256;
        new_w = static_cast<int>(w * 256.0 / h);
    } else {
        new_w = 256;
        new_h = static_cast<int>(h * 256.0 / w);
    }
    cv::resize(img, img, cv::Size(new_w, new_h));

    // 中心裁剪 224x224
    int x = (img.cols - 224) / 2;
    int y = (img.rows - 224) / 2;
    cv::Rect roi(x, y, 224, 224);
    cv::Mat crop = img(roi);

    // 转 float32 [0,1]
    cv::Mat floatImg;
    crop.convertTo(floatImg, CV_32F, 1.0 / 255.0);

    // 标准化
    std::vector<float> mean = {0.485f, 0.456f, 0.406f};
    std::vector<float> std  = {0.229f, 0.224f, 0.225f};
    std::vector<cv::Mat> channels(3);
    cv::split(floatImg, channels);
    for (int i = 0; i < 3; i++) {
        channels[i] = (channels[i] - mean[i]) / std[i];
    }
    cv::merge(channels, floatImg);

    // 7. HWC -> CHW
    cv::split(floatImg, channels);
    std::vector<float> chw;
    chw.reserve(3 * 224 * 224);
    for (int i = 0; i < 3; i++) {
        chw.insert(chw.end(), (float*)channels[i].datastart, (float*)channels[i].dataend);
    }

    // 8. float32 -> uint8_t（字节序列）
    std::vector<uint8_t> bytes(chw.size() * sizeof(float));
    std::memcpy(bytes.data(), chw.data(), chw.size() * sizeof(float));

    return bytes; // 返回的字节序列，内部其实是 float32
}
    
*/


std::vector<uint8_t> lenetPreprocess(const std::any& arg) {
    auto path = std::any_cast<std::string>(&arg);
    assert(path != nullptr);
    std::ifstream binFile(*path, std::ifstream::binary);
    if (binFile.is_open() != true) {
        ERROR_LOG("file is not opened");
        return {};
    }
    binFile.seekg(0, binFile.end);
    uint32_t binFileBufferLen = binFile.tellg();
    if (binFileBufferLen == 0 || binFileBufferLen != 784) {
        ERROR_LOG("file size error");
        return {};
    }
    binFile.seekg(0,binFile.beg);
    uint8_t *hostBuf = (uint8_t *)malloc(784);
    if (nullptr != hostBuf) {
        binFile.read((char *)(hostBuf), binFileBufferLen);
    }
    std::vector<uint8_t> result(hostBuf, hostBuf + binFileBufferLen);
    free(hostBuf);

    return result;
}


int main() {
    std::string yaml = "../lenet.yaml";
    Session s2(yaml);
    s2.registerPreprocess(lenetPreprocess);
    auto outputs = s2.Run();
    for (int i = 0; i < outputs.size(); i++) {
        printf("task[%d]'s output:\n", i);
        for (auto task_out : outputs[i]) {
            printVector(bytesToUint16(task_out));
        }
    }
}
