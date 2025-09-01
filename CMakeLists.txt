cmake_minimum_required(VERSION 3.10)
project(InferenceProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LYNXI "Enable Lynxi backend" ON)
option(ACL   "Enable Ascend ACL backend" OFF)

message(STATUS "====================================")
message(STATUS " Backend selection:")
message(STATUS "   LYNXI backend: ${LYNXI}")
message(STATUS "   ACL backend:   ${ACL}")
message(STATUS "====================================")

set(LIB_SRC
    src/framework/executor.cc
    src/framework/monitor.cc
    src/framework/session.cc
    src/util.cc
    src/framework/backend/dummy.cc
)

if(LYNXI)
    list(APPEND LIB_SRC src/framework/backend/lynxi.cc)
endif()

if(ACL)
    list(APPEND LIB_SRC src/framework/backend/ascend.cc)
endif()

add_library(inference SHARED ${LIB_SRC})

target_link_directories(inference
    PUBLIC
        ${CMAKE_SOURCE_DIR}/3rd_party/opencv/lib
        ${CMAKE_SOURCE_DIR}/3rd_party/yaml/lib/arm
)

if(LYNXI)
    target_link_directories(inference PUBLIC
        ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/lib
    )
    if(EXISTS "/usr/local/lynxi/sdk/libs")
        target_link_directories(inference PUBLIC /usr/local/lynxi/sdk/libs)
    endif()
endif()

if(ACL)
    target_link_directories(inference PUBLIC
        ${CMAKE_SOURCE_DIR}/3rd_party/acl/lib
    )
endif()

target_link_libraries(inference
    PUBLIC
        pthread
        yaml-cpp
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
)

if(LYNXI)
    target_link_libraries(inference
        PUBLIC
            LYNCHIPSDKCLIENT
            LYNSMICLIENTCOMM
    )
endif()

if(ACL)
    target_link_libraries(inference
        PUBLIC
            ascendcl
    )
endif()

target_include_directories(inference
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/3rd_party/opencv/include/opencv4
        ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/include
        ${CMAKE_SOURCE_DIR}/3rd_party/yaml/include
        ${CMAKE_SOURCE_DIR}/3rd_party/acl/include

)

# if(LYNXI)
#     target_include_directories(inference PUBLIC
#         ${CMAKE_SOURCE_DIR}/3rd_party/lynxi/include
#     )
#     if(EXISTS "/usr/local/lynxi/sdk/include")
#         target_include_directories(inference PUBLIC /usr/local/lynxi/sdk/include)
#     endif()
# endif()

# if(ACL)
#     target_include_directories(inference PUBLIC
#         ${CMAKE_SOURCE_DIR}/3rd_party/acl/include
#     )
# endif()


set(APP_SRCS
    src/app/resnet.cc
    src/app/lenet.cc
    src/app/yolo.cc
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
