# toolchain-arm.cmake

# 指定交叉编译器
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译器路径
SET(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
# SET(CMAKE_CXX_COMPILER /home/anby/workspace/gcc-build/gcc-install/bin/aarch64-linux-gnu-g++)
SET(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)

# 可选：设置 sysroot（目标系统的根文件系统）
# SET(CMAKE_SYSROOT /usr/aarch64-linux-gnu)

# 可选：禁用本地路径的编译检测（防止使用本地头文件）
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
