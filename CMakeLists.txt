cmake_minimum_required(VERSION 3.10)
project(InferenceProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SRC
    src/executor.cc
    src/lynxi.cc
    src/monitor.cc
    src/session.cc
    src/util.cc
    src/dummy.cc
)

add_library(inference SHARED ${LIB_SRC})

target_link_directories(inference
    PUBLIC
        ${CMAKE_SOURCE_DIR}/3rd_party/opencv/lib
        ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/lib
        ${CMAKE_SOURCE_DIR}/3rd_party/yaml/lib/arm
        /usr/local/lynxi/sdk/libs
)

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

target_include_directories(inference
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rd_party/opencv/include/opencv4
    ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/include
    ${CMAKE_SOURCE_DIR}/3rd_party/yaml/include
    /usr/local/lynxi/sdk/include
)

set(APP_SRCS
    src/resnet.cc
    src/lenet.cc
)

foreach(app_src ${APP_SRCS})
    get_filename_component(app_name ${app_src} NAME_WE)
    add_executable(${app_name} ${app_src})
    target_link_libraries(${app_name} inference)
    set_target_properties(${app_name} PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/usr/lib/aarch64-linux-gnu"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
endforeach()
