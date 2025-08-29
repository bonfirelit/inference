#include "session.h"
#include <fstream>


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
