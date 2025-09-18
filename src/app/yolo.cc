#include "session.h"
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <map>

struct Detection {
    float x0, y0, x1, y1; // 坐标
    float score;          // 置信度
    int class_id;         // 类别id
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

std::vector<uint8_t> preprocess(const std::any& arg);
float IoU(const Detection& a, const Detection& b);
std::vector<Detection> NMS(const std::vector<Detection>& dets, float iou_threshold);
std::vector<Detection> yolo_postprocess(const float* output,
                                        int num_boxes, int num_classes,
                                        float conf_thresh, float iou_thresh,
                                        int input_w, int input_h,
                                        int orig_w, int orig_h);


int main() {
    std::string imgpath = "../../yolov5/test.jpg";
    // cv::Mat img = cv::imread("../../yolov4/data/dog.jpg");
    std::string yaml = "../yolov5.yaml";
    Session s2(yaml);
    s2.registerPreprocess(preprocess);
    auto outputs = s2.Run();
    // post process
    std::cout << "start to post process" << std::endl;
    cv::Mat img = cv::imread(imgpath);
    if (img.empty()) {
        std::cerr << "cannot read image\n";
        return -1;
    }

    std::cout << "length = " << outputs[0][0].size() << std::endl;
    std::vector<uint8_t>& output_bytes = outputs[0][0];
    float* output_floats = reinterpret_cast<float*>(output_bytes.data());
    int num_classes = 80;
    int num_boxes = output_bytes.size() / (sizeof(float) * (num_classes + 5));
    int input_w = 640, input_h = 640;
    int orig_w = img.cols, orig_h = img.rows;

    std::cout << "post process" << std::endl;

    auto dets = yolo_postprocess(output_floats, 12600, num_classes,
                                0.25f, 0.45f, // conf, iou
                                input_w, input_h,
                                orig_w, orig_h);

    // 画框
    for (auto& d : dets) {
        cv::rectangle(img, cv::Point((int)d.x0, (int)d.y0),
                      cv::Point((int)d.x1, (int)d.y1),
                      cv::Scalar(0, 0, 255), 2);
        char text[100];
        sprintf(text, "cls:%d %.2f", d.class_id, d.score);
        cv::putText(img, text, cv::Point((int)d.x0, (int)d.y0 - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0,255,0), 1);
    }

    cv::imwrite("res.jpg", img);
    std::cout << "Result saved to res.jpg\n";

    return 0;
}

std::vector<uint8_t> preprocess(const std::any& arg) {
    int input_w = 640, input_h = 640;
    auto image_path = std::any_cast<std::string>(&arg);
    if (image_path == nullptr) {
        return {};
    }
    // 1. 读取图片
    cv::Mat img = cv::imread(*image_path);
    if (img.empty()) {
        throw std::runtime_error("Failed to read image: " + *image_path);
    }

    // 2. BGR -> RGB
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

    // 3. resize
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(input_w, input_h));

    // 4. 转为 float32，归一化到 [0,1]
    resized.convertTo(resized, CV_32FC3, 1.0 / 255.0);

    // 5. HWC -> CHW
    std::vector<cv::Mat> channels(3);
    cv::split(resized, channels);  // [H,W] x 3
    std::vector<float> chw;
    for (int i = 0; i < 3; i++) {
        chw.insert(chw.end(), (float*)channels[i].datastart, (float*)channels[i].dataend);
    }

    // 6. 转为字节数组 (uint8_t)
    std::vector<uint8_t> data(chw.size() * sizeof(float));
    memcpy(data.data(), chw.data(), chw.size() * sizeof(float));

    return data;
}

float IoU(const Detection& a, const Detection& b) {
    float xx1 = std::max(a.x0, b.x0);
    float yy1 = std::max(a.y0, b.y0);
    float xx2 = std::min(a.x1, b.x1);
    float yy2 = std::min(a.y1, b.y1);

    float w = std::max(0.0f, xx2 - xx1);
    float h = std::max(0.0f, yy2 - yy1);
    float inter = w * h;
    float areaA = (a.x1 - a.x0) * (a.y1 - a.y0);
    float areaB = (b.x1 - b.x0) * (b.y1 - b.y0);
    return inter / (areaA + areaB - inter + 1e-6f);
}

std::vector<Detection> NMS(const std::vector<Detection>& dets, float iou_threshold) {
    std::vector<Detection> result;
    std::vector<Detection> sorted = dets;

    std::sort(sorted.begin(), sorted.end(),
              [](const Detection& a, const Detection& b) { return a.score > b.score; });

    std::vector<bool> removed(sorted.size(), false);

    for (size_t i = 0; i < sorted.size(); i++) {
        if (removed[i]) continue;
        result.push_back(sorted[i]);
        for (size_t j = i + 1; j < sorted.size(); j++) {
            if (removed[j]) continue;
            if (IoU(sorted[i], sorted[j]) > iou_threshold) {
                removed[j] = true;
            }
        }
    }
    return result;
}

std::vector<Detection> yolo_postprocess(const float* output,
                                        int num_boxes, int num_classes,
                                        float conf_thresh, float iou_thresh,
                                        int input_w, int input_h,
                                        int orig_w, int orig_h) {
    std::vector<Detection> detections;

    INFO_LOG("b1");
    for (int i = 0; i < num_boxes; i++) {
        const float* ptr = output + i * (num_classes + 5);

        // INFO_LOG("b2");
        float cx = ptr[0];
        float cy = ptr[1];
        float w  = ptr[2];
        float h  = ptr[3];
        float obj_conf = sigmoid(ptr[4]);

        if (obj_conf < conf_thresh) continue;

        // INFO_LOG("b3");
        // 找最大类别概率
        float max_prob = 0.0f;
        int class_id = -1;
        for (int c = 0; c < num_classes; c++) {
            float p = sigmoid(ptr[5 + c]);
            if (p > max_prob) {
                max_prob = p;
                class_id = c;
            }
        }

        float score = obj_conf * max_prob;
        if (score < conf_thresh) continue;

        float x0 = cx - w / 2.0f;
        float y0 = cy - h / 2.0f;
        float x1 = cx + w / 2.0f;
        float y1 = cy + h / 2.0f;

        // 映射回原图
        float scale = std::min((float)input_w / orig_w, (float)input_h / orig_h);
        float pad_w = (input_w - orig_w * scale) / 2;
        float pad_h = (input_h - orig_h * scale) / 2;

        x0 = (x0 - pad_w) / scale;
        x1 = (x1 - pad_w) / scale;
        y0 = (y0 - pad_h) / scale;
        y1 = (y1 - pad_h) / scale;

        detections.push_back({x0, y0, x1, y1, score, class_id});
    }

    return NMS(detections, iou_thresh);
}