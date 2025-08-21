#include "session.h"
#include <cstring>


void printVector(std::vector<float> v) {
    std::cout << "[ ";
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::string yaml = "../dummy_config.yaml";
    Session s2(yaml);
    int i = 0;
    int shape_in_yaml = 5;
    s2.RegisterPreprocess([&]() -> std::vector<uint8_t> {
        std::vector<float> input(shape_in_yaml, (float)(i));
        i++;
        std::vector<uint8_t> ret(input.size() * sizeof(float));
        std::memcpy(ret.data(), input.data(), input.size() * sizeof(float));
        return ret;
    });
    auto outputs = s2.Run();
    for (int i = 0; i < outputs.size(); i++) {
        printf("task[%d]'s output:\n", i);
        for (auto task_out : outputs[i]) {
            printVector(task_out);
        }
    }
}
