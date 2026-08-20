# toolchain-aarch64.cmake —— RK3568 交叉编译工具链
# 用法: cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-aarch64.cmake ..
# 编译器: Linaro 6.3.1 (aarch64-linux-gnu)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 交叉编译器（PATH 里已前置 Linaro 6.3.1）
set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_STRIP        aarch64-linux-gnu-strip)

# 查找规则：程序用宿主的，库/头文件只找目标机的
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
