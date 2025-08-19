#include "session.h"
#include <cstring>
int main() {
    int num_executor = 1;
    Session session(BACKEND_DUMMY, num_executor, "dummy", "dummy");
    std::vector<float> input{1.0, 2.0, 3.0, 4.0, 5.0};
    session.RegisterPreprocess([&]() -> std::vector<uint8_t> {
        std::vector<uint8_t> ret(input.size() * sizeof(float));
        std::memcpy(ret.data(), input.data(), input.size() * sizeof(float));
        return ret;
    });
    auto output = session.Run();
    std::cout << "The output is: " << std::endl;
    for (auto x : output) {
        std::cout << x << std::endl;
    }
}