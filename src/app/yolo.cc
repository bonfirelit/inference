#include "session.h"
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <map>

struct Detection {
    int class_id;
    float confidence;
    cv::Rect box;
};

std::vector<std::string> class_names = {
    "person","bicycle","car","motorcycle","airplane","bus","train","truck","boat",
    "traffic light","fire hydrant","stop sign","parking meter","bench","bird",
    "cat","dog","horse","sheep","cow","elephant","bear","zebra","giraffe",
    "backpack","umbrella","handbag","tie","suitcase","frisbee","skis","snowboard","sports ball",
    "kite","baseball bat","baseball glove","skateboard","surfboard","tennis racket",
    "bottle","wine glass","cup","fork","knife","spoon","bowl","banana","apple",
    "sandwich","orange","broccoli","carrot","hot dog","pizza","donut","cake",
    "chair","couch","potted plant","bed","dining table","toilet","tv","laptop","mouse",
    "remote","keyboard","cell phone","microwave","oven","toaster","sink","refrigerator",
    "book","clock","vase","scissors","teddy bear","hair drier","toothbrush"
};

inline float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }


std::vector<uint8_t> yolov4Preprocess(const std::any& arg);
std::vector<Detection> nms(const std::vector<Detection>& dets, float nms_thresh);
std::vector<Detection> nms_per_class(const std::vector<Detection>& dets, float nms_thresh);
std::vector<Detection> postprocess(const std::vector<std::vector<float>>& tensor,
                                   int input_width = 416, int input_height = 416,
                                   float conf_threshold = 0.5, float nms_threshold = 0.5);

void drawDetections(cv::Mat& image, const std::vector<Detection>& detections,
                    const std::vector<std::string>& class_names);

int main() {
    cv::Mat img = cv::imread("../../yolov4/data/dog.jpg");
    std::string yaml = "../yolov4.yaml";
    Session s2(yaml);
    s2.registerPreprocess(yolov4Preprocess);
    auto outputs = s2.Run();
    for (int i = 0; i < outputs.size(); i++) {
        printf("task[%d]'s output:\n", i);
        // 对于yolov4，out的长度为3，代表有三个输出张量
        auto out = outputs[i];
        // 转换输出并做 sigmoid
        std::vector<std::vector<float>> outputs_float;

        for (size_t i = 0; i < out.size(); i++) {
            // 1. 字节转 float
            std::vector<float> out_f = bytesToFloat32(out[i]);

            // 2. 确定特征图 H、W
            int H, W;
            size_t total = out_f.size();
            if (total == 52*52*3*85) { H=W=52; }
            else if (total == 26*26*3*85) { H=W=26; }
            else if (total == 13*13*3*85) { H=W=13; }
            else { 
                ERROR_LOG("Unexpected tensor size: %ld", total);
                // assert(0);
                continue;
            }

            // 3. 对 obj_score 和 class_prob 做 sigmoid
            for (int y = 0; y < H; y++) {
                for (int x = 0; x < W; x++) {
                    for (int a = 0; a < 3; a++) {
                        int idx = ((y*W + x)*3 + a)*85;

                        // obj_score
                        out_f[idx+4] = sigmoid(out_f[idx+4]);

                        // class_prob
                        for (int c = 0; c < 80; c++) {
                            out_f[idx+5+c] = sigmoid(out_f[idx+5+c]);
                        }
                    }
                }
            }

            outputs_float.push_back(std::move(out_f));
        }
        std::vector<Detection> detections = postprocess(outputs_float, 416, 416, 0.35, 0.5);
        drawDetections(img, detections, class_names);
        cv::imwrite("result.jpg", img);
    }
    return 0;
}

// 非极大值抑制 NMS
std::vector<Detection> nms(const std::vector<Detection>& dets, float nms_thresh) {
    std::vector<Detection> result;
    std::vector<Detection> sorted = dets;
    std::sort(sorted.begin(), sorted.end(), [](const Detection& a, const Detection& b) {
        return a.confidence > b.confidence;
    });

    std::vector<bool> removed(sorted.size(), false);
    for (size_t i = 0; i < sorted.size(); i++) {
        if (removed[i]) continue;
        result.push_back(sorted[i]);
        for (size_t j = i + 1; j < sorted.size(); j++) {
            if (removed[j]) continue;
            float inter_area = (sorted[i].box & sorted[j].box).area();
            float union_area = (sorted[i].box | sorted[j].box).area();
            if (inter_area / union_area > nms_thresh) removed[j] = true;
        }
    }
    return result;
}

std::vector<Detection> postprocess(const std::vector<std::vector<float>>& outputs,
                                   int input_width,
                                   int input_height,
                                   float conf_thresh,
                                   float nms_thresh) {
    // YOLOv4 COCO anchors（对应 52x52,26x26,13x13）
    std::vector<std::vector<cv::Size2f>> anchors = {
        {{10,13}, {16,30}, {33,23}},    // 52x52
        {{30,61}, {62,45}, {59,119}},   // 26x26
        {{116,90}, {156,198}, {373,326}} // 13x13
    };
    std::vector<int> strides = {8, 16, 32}; // 对应特征图尺度

    std::vector<Detection> detections;

    for (size_t i = 0; i < outputs.size(); i++) {
        const std::vector<float>& data = outputs[i]; // 已经是 float vector，包含 sigmoid 处理

        int H = (i == 0 ? 52 : (i == 1 ? 26 : 13));
        int W = H;

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                for (int a = 0; a < 3; a++) {
                    int idx = ((y * W + x) * 3 + a) * 85;

                    float tx = data[idx + 0];
                    float ty = data[idx + 1];
                    float tw = data[idx + 2];
                    float th = data[idx + 3];
                    float obj_score = data[idx + 4]; // 已经 sigmoid

                    // 类别概率
                    float max_class_prob = 0.0f;
                    int class_id = -1;
                    for (int c = 0; c < 80; c++) {
                        float prob = data[idx + 5 + c]; // 已经 sigmoid
                        if (prob > max_class_prob) {
                            max_class_prob = prob;
                            class_id = c;
                        }
                    }

                    float confidence = obj_score * max_class_prob;
                    if (confidence < conf_thresh) continue;

                    // 解码 bbox
                    float cx = (tx + x) * strides[i]; // 假设 tx 已经 sigmoid
                    float cy = (ty + y) * strides[i];
                    float w  = std::exp(tw) * anchors[i][a].width;
                    float h  = std::exp(th) * anchors[i][a].height;

                    int left   = std::max(0, int(cx - w/2));
                    int top    = std::max(0, int(cy - h/2));
                    int right  = std::min(input_width - 1, int(cx + w/2));
                    int bottom = std::min(input_height - 1, int(cy + h/2));
                    if (bottom <= top || right <= left) continue;

                    std::cout << "class_id=" << class_id
                              << " conf=" << confidence
                              << " box=(" << left << "," << top << "," << right << "," << bottom << ")\n";

                    detections.push_back({class_id, confidence, cv::Rect(left, top, right-left, bottom-top)});
                }
            }
        }
    }

    return nms_per_class(detections, nms_thresh);
}

std::vector<Detection> nms_per_class(const std::vector<Detection>& dets, float nms_thresh) {
    std::map<int, std::vector<Detection>> cls_map;
    for (auto& d : dets) cls_map[d.class_id].push_back(d);

    std::vector<Detection> result;
    for (auto& kv : cls_map) {
        auto& cls_dets = kv.second;
        std::sort(cls_dets.begin(), cls_dets.end(), [](auto& a, auto& b){ return a.confidence > b.confidence; });
        std::vector<bool> removed(cls_dets.size(), false);
        for (size_t i = 0; i < cls_dets.size(); i++) {
            if (removed[i]) continue;
            result.push_back(cls_dets[i]);
            for (size_t j = i+1; j < cls_dets.size(); j++) {
                if (removed[j]) continue;
                float inter_area = (cls_dets[i].box & cls_dets[j].box).area();
                float union_area = (cls_dets[i].box | cls_dets[j].box).area();
                if (inter_area/union_area > nms_thresh) removed[j] = true;
            }
        }
    }
    return result;
}


std::vector<uint8_t> yolov4Preprocess(const std::any& arg) {
    auto path = std::any_cast<std::string>(&arg);
    if (path == nullptr) {
        return {};
    }
    int input_width = 416, input_height = 416;
    // 1. 读取图像
    cv::Mat img = cv::imread(*path);
    assert(!img.empty());

    // 2. BGR -> RGB
    cv::Mat rgb;
    cv::cvtColor(img, rgb, cv::COLOR_BGR2RGB);

    // 3. resize 到 (416,416)
    cv::Mat resized;
    cv::resize(rgb, resized, cv::Size(input_width, input_height));

    // 4. 转 float32 并归一化 [0,1]
    resized.convertTo(resized, CV_32F, 1.0 / 255.0);

    // 5. NHWC 格式 + batch 维度
    cv::Mat inputBlob = resized.reshape(1, {1, input_height, input_width, 3}); // [1,416,416,3]

    // 6. 转换为 vector<uint8_t>
    size_t data_size = inputBlob.total() * inputBlob.elemSize(); // float32 占 4 字节
    std::vector<uint8_t> buffer(data_size);
    std::memcpy(buffer.data(), inputBlob.data, data_size);

    return buffer;
}


// class_names 是类别名字列表，比如 {"person", "bicycle", ...}
void drawDetections(cv::Mat& image, const std::vector<Detection>& detections,
                    const std::vector<std::string>& class_names) {
    for (const auto& det : detections) {
        // 随机颜色（这里用 class_id 来决定颜色，保证同类颜色一致）
        cv::Scalar color(50 * (det.class_id % 5), 80 * (det.class_id % 3), 100 * (det.class_id % 7));

        // 1. 画框
        cv::rectangle(image, det.box, color, 2);

        // 2. 文字：类别 + 置信度
        std::string label = class_names[det.class_id] + " " + cv::format("%.2f", det.confidence);
        int baseLine = 0;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = std::max(det.box.y, labelSize.height);

        // 画背景
        cv::rectangle(image, cv::Point(det.box.x, top - labelSize.height),
                      cv::Point(det.box.x + labelSize.width, top + baseLine),
                      color, cv::FILLED);

        // 写字
        cv::putText(image, label, cv::Point(det.box.x, top),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1);
    }
}