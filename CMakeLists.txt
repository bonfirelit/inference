cmake_minimum_required(VERSION 3.10)
project(InferenceProject LANGUAGES CXX)

# 使用 C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 库文件目录
link_directories(
    ${CMAKE_SOURCE_DIR}/3rd_party/opencv/lib
    ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/lib
)

set(LIB_SRC
    src/executor.cc
    src/lynxi.cc
    src/monitor.cc
    src/session.cc
    src/dummy.cc
)

set(MAIN_SRC
    src/main.cc
)

# 构建动态库
add_library(inference SHARED ${LIB_SRC})
target_link_libraries(inference PUBLIC pthread)

# 构建可执行文件
add_executable(inference_app ${MAIN_SRC})

target_include_directories(inference
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rd_party/opencv/include/opencv4
    ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/include
)

链接第三方库到 inference
target_link_libraries(inference
    opencv_core
    opencv_imgproc
    opencv_imgcodecs
    # LYNXICHIPSDKCLIENT
)

# 链接动态库到可执行程序
target_link_libraries(inference_app inference)
