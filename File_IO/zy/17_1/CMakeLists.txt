cmake_minimum_required(VERSION 3.16)
project(show_image C)

set(CMAKE_C_STANDARD 11)

# 第三方解码库根目录
set(THIRDLIB ${CMAKE_SOURCE_DIR}/thirdlib)

# 头文件搜索路径（lcd.h 在当前目录 + 四个库的头文件）
include_directories(${CMAKE_SOURCE_DIR}
                    ${THIRDLIB}/jpeg/include
                    ${THIRDLIB}/png/include
                    ${THIRDLIB}/gif/include
                    ${THIRDLIB}/zlib/include)

# 库文件搜索路径
link_directories(${THIRDLIB}/jpeg/lib
                 ${THIRDLIB}/png/lib
                 ${THIRDLIB}/gif/lib
                 ${THIRDLIB}/zlib/lib)

# ---- 主程序：dlopen 架构只需要 -ldl ----
add_executable(main main.c lcd.c)
target_link_libraries(main dl)

# ---- 插件库：全部编译成 .so ----
# BMP：无第三方依赖
add_library(bmp SHARED bmp.c)

# JPG：链接 libjpeg
add_library(jpg SHARED jpg.c)
target_link_libraries(jpg jpeg)

# PNG：链接 libpng16 + zlib（target 名 png，库名 png16/z，不撞车）
add_library(png SHARED png.c)
set_target_properties(png PROPERTIES OUTPUT_NAME pngimg)
target_link_libraries(png png16 z)

# GIF：链接 giflib（注意：target 名不能用 gif，会和库名 gif 撞车 → target 命名 gifimg）
add_library(gifimg SHARED gif.c)
target_link_libraries(gifimg gif)
