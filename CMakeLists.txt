cmake_minimum_required(VERSION 3.10)
project(InferenceProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


link_directories(
    ${CMAKE_SOURCE_DIR}/3rd_party/opencv/lib
    ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/lib
    ${CMAKE_SOURCE_DIR}/3rd_party/yaml/lib/arm
    /usr/local/lynxi/sdk/libs
    # ${CMAKE_SOURCE_DIR}/3rd_party/yaml/lib/x86
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

add_library(inference SHARED ${LIB_SRC})
target_link_libraries(inference
    PUBLIC
        pthread
        yaml-cpp
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        LYNCHIPSDKCLIENT
        LYNSMICLIENTCOMM
)
add_executable(inference_app ${MAIN_SRC})

target_include_directories(inference
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rd_party/opencv/include/opencv4
    ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/include
    ${CMAKE_SOURCE_DIR}/3rd_party/yaml/include
    /usr/local/lynxi/sdk/include
)

target_link_libraries(inference_app inference)

set_target_properties(inference_app PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "/usr/lib/aarch64-linux-gnu"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)