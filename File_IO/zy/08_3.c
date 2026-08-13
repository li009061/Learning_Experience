// 编写一个程序，使得在LCD上显示八个不同的单色色块，并呈逆时针流转显示。
// 08_3.c：高效填充版 —— 逐行数组写，编译器自动优化成块拷贝
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// 功能：把 (row, col) 位置的色块填成 color（高效版）
// 原理：逐行填充。行内用数组下标 line[x] = color，
//       编译器会把连续写自动优化成块拷贝指令（NEON/循环展开）
static void fill_block(char *fb, int row, int col, int block_w, int block_h, int line_size, uint32_t color)
{
    for (int y = row * block_h; y < (row + 1) * block_h; y++) {
        // 本行色块的起始像素地址（按 uint32_t 数组看待）
        uint32_t *line = (uint32_t *)(fb + y * line_size + col * block_w * 4);
        for (int x = 0; x < block_w; x++) {
            line[x] = color;      // 连续写，编译器自动向量化
        }
    }
}

//功能：每隔一秒，轮转显示八种不同的单色色块
int main(void)
{
    //打开LCD帧缓冲设备
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1){
        perror("open lcd failed");
        return -1;
    }

    // 获取LCD设备的硬件参数
    struct fb_var_screeninfo lcd_info;
    if (ioctl(lcd, FBIOGET_VSCREENINFO, &lcd_info) == -1) {
        perror("ioctl failed");
        close(lcd);
        return -1;
    }

    // 计算LCD设备的像素地址
    int lcd_w = lcd_info.xres;
    int lcd_h = lcd_info.yres;
    int lcd_bpp = lcd_info.bits_per_pixel;  //色深
    int screen_size = lcd_w * lcd_h * lcd_bpp / 8;
    int lcd_line_size = lcd_w * lcd_bpp / 8;

    // 映射LCD设备的像素地址到用户空间
    char *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED){
        perror("mmap failed");
        close(lcd);
        return -1;
    }

    // 定义八种颜色红，黄，棕，粉，绿，蓝，白，黑
    uint32_t colors[] = {0xFF0000, 0xFFFF00, 0x8B4513, 0xFFC0CB, 0x008000, 0x0000FF, 0xFFFFFF, 0x000000};

    //算出每个色块的宽度和高度
    int block_w = lcd_w / 4;
    int block_h = lcd_h / 2;

    // 每隔一秒，轮转显示八种不同的单色色块（逆时针）
    for (int t = 0; ; t++) {
        for (int i = 0; i < 8; i++) {
            int row = i / 4;              // 0=顶行 1=底行
            int col = i % 4;              // 0~3列
            uint32_t color = colors[(i - t + 8) % 8];   // 逆时针
            fill_block(p, row, col, block_w, block_h, lcd_line_size, color);
        }
        sleep(1);
    }

    return 0;
}