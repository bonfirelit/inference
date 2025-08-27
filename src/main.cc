#include "session.h"
#include <cstring>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

void printVector(std::vector<float> v) {
    std::cout << "[ ";
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << "]" << std::endl;
}

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

int main() {
    std::string yaml = "../dummy_config.yaml";
    Session s2(yaml);
    int i = 0;
    int shape_in_yaml = 5;
    // s2.registerPreprocess([&]() -> std::vector<uint8_t> {
    //     std::vector<float> input(shape_in_yaml, (float)(i));
    //     i++;
    //     std::vector<uint8_t> ret(input.size() * sizeof(float));
    //     std::memcpy(ret.data(), input.data(), input.size() * sizeof(float));
    //     return ret;
    // });
    s2.registerPreprocess(resnetPreprocess);
    auto outputs = s2.Run();
    for (int i = 0; i < outputs.size(); i++) {
        printf("task[%d]'s output:\n", i);
        for (auto task_out : outputs[i]) {
            printVector(task_out);
        }
    }
}
