// 编写一个程序，使得在LCD上显示八个不同的单色色块，并呈逆时针流转显示。
// 程序功能：每隔一秒，轮转显示八种不同的单色色块，形成逆时针流转效果

#include <stdio.h>      // 标准输入输出库，提供 perror、printf 等函数
#include <fcntl.h>      // 文件控制选项，提供 open 函数及相关宏
#include <unistd.h>     // UNIX 标准函数库，提供 close、sleep 等函数
#include <sys/ioctl.h>  // 设备控制接口，提供 ioctl 函数
#include <linux/fb.h>   // Linux 帧缓冲设备头文件，定义 fb_var_screeninfo 等结构体
#include <linux/videodev2.h> // V4L2 视频设备头文件（本程序未使用）
#include <sys/mman.h>   // 内存映射库，提供 mmap、munmap 函数
#include <sys/stat.h>   // 文件状态库（本程序未使用）
#include <sys/types.h>  // 基本系统数据类型
#include <string.h>     // 字符串操作库，提供 memcpy 函数
#include <stdint.h>     // 标准整数类型库，提供 uint32_t 等类型
#include <stdlib.h>     // 标准库（本程序未使用）
#include <time.h>       // 时间库（本程序未使用）
#include <sys/select.h> // select 多路复用库（本程序未使用）



// 功能：每隔一秒，轮转显示八种不同的单色色块
int main(int argc, char const *argv[])
{
    // 打开 LCD 帧缓冲设备 /dev/fb0
    // O_RDWR：以可读可写方式打开，因为我们需要向屏幕写入颜色数据
    int lcd = open("/dev/fb0", O_RDWR);   
    if(lcd == -1){
        perror("open lcd failed");  // 打开失败时打印错误信息
        return -1;
    }

    // 定义帧缓冲可变信息结构体，用于获取 LCD 屏幕的宽度、高度、色深等参数
    struct fb_var_screeninfo lcd_info;
    
    // FBIOGET_VSCREENINFO：获取帧缓冲设备的可变屏幕信息
    // 调用成功后将屏幕参数填充到 lcd_info 结构体中
    ioctl(lcd, FBIOGET_VSCREENINFO, &lcd_info);

    // 从获取到的参数中解析屏幕信息
    int lcd_w = lcd_info.xres;          // 屏幕水平分辨率（宽度，单位：像素）
    int lcd_h = lcd_info.yres;          // 屏幕垂直分辨率（高度，单位：像素）
    int lcd_bpp = lcd_info.bits_per_pixel;  // 色深，即每个像素占用的二进制位数
    int screen_size = lcd_w * lcd_h * lcd_bpp / 8;  // 整个屏幕占用的字节数
    int lcd_line_size = lcd_w * lcd_bpp / 8;        // 屏幕一行像素占用的字节数

    // 将 LCD 帧缓冲内存映射到用户空间
    // 参数说明：
    //   NULL：让系统自动选择映射起始地址
    //   screen_size：映射区域大小（整个屏幕的字节数）
    //   PROT_READ | PROT_WRITE：映射区域可读可写
    //   MAP_SHARED：对映射区域的修改会同步回硬件设备
    //   lcd：帧缓冲设备的文件描述符
    //   0：映射起始偏移量
    // 返回值 p 是映射到用户空间的内存首地址，后续可通过 p 直接操作屏幕像素
    char *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED){
        perror("mmap failed");  // 映射失败时打印错误信息
        close(lcd);             // 关闭 LCD 设备文件
        return -1;
    }

    // 定义八种颜色，每个颜色用一个 32 位无符号整数表示
    // 颜色格式为 ARGB 或 RGB（具体取决于 LCD 驱动），此处按 0xRRGGBB 格式填写
    // 依次是：红、黄、棕、粉、绿、蓝、白、黑
    uint32_t colors[] = {0xFF0000, 0xFFFF00, 0x8B4513, 0xFFC0CB, 0x008000, 0x0000FF, 0xFFFFFF, 0x000000};
    
    // 将屏幕划分为 4 列 × 2 行，共 8 个色块
    // 每个色块的宽度为屏幕宽度的 1/4
    // 每个色块的高度为屏幕高度的 1/2
    int block_w = lcd_w / 4;
    int block_h = lcd_h / 2;

    // 外层无限循环：每一轮 k 增加 1，八种颜色整体逆时针轮转一个位置
    // 每个色块显示 colors[(k+n)%8]，其中 n 为色块编号
    for(int k = 0;;k++){
        
        // 遍历每个色块内部的每一个像素
        // i 表示色块内的垂直偏移（0 到 block_h-1）
        // j 表示色块内的水平偏移（0 到 block_w-1）
        for(int i = 0; i < block_h; i++){
            for(int j =0; j < block_w; j++){
                
                // 帧缓冲中像素地址计算公式：
                // 水平方向偏移：4 * (j + n * block_w)，其中 n 为列号，4 表示每个像素 4 字节（32 位色深）
                // 垂直方向偏移：lcd_line_size * (i + m * block_h)，其中 m 为行号
                // 总偏移 = 水平偏移 + 垂直偏移
                // 通过 memcpy 将颜色值复制到对应像素位置

                // 第一行色块（上半部分，m = 0）
                memcpy(p+4*(j+0*block_w)+lcd_line_size*(i+0*block_h), &colors[(k+0)%8], 4);
                memcpy(p+4*(j+1*block_w)+lcd_line_size*(i+0*block_h), &colors[(k+1)%8], 4);
                memcpy(p+4*(j+2*block_w)+lcd_line_size*(i+0*block_h), &colors[(k+2)%8], 4);
                memcpy(p+4*(j+3*block_w)+lcd_line_size*(i+0*block_h), &colors[(k+3)%8], 4);

                // 第二行色块（下半部分，m = 1）
                memcpy(p+4*(j+0*block_w)+lcd_line_size*(i+1*block_h), &colors[(k+4)%8], 4);
                memcpy(p+4*(j+1*block_w)+lcd_line_size*(i+1*block_h), &colors[(k+5)%8], 4);
                memcpy(p+4*(j+2*block_w)+lcd_line_size*(i+1*block_h), &colors[(k+6)%8], 4);
                memcpy(p+4*(j+3*block_w)+lcd_line_size*(i+1*block_h), &colors[(k+7)%8], 4);
            }
        }
        
        // 每次刷新完一屏后暂停 1 秒，然后进入下一轮，实现色块流转效果
        sleep(1);
    }
    
    return 0;
}
