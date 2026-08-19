#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "png.h"    // libpng 头文件(高层 API png_image 系列)
#include "lcd.h"

/*
 * show_image —— 显示 PNG 图片到 LCD
 *
 * 思路:
 *   1. 用 libpng 高层 API png_image 把 PNG 解码成 RGB24 数据
 *      (自动完成 zlib 解压、PNG 滤波、位深/颜色类型转换);
 *   2. 把 RGB24 逐像素转成 LCD 的 BGRX32 格式写入显存;
 *   3. 图片在屏幕上居中显示,超出屏幕的部分截断。
 */
void show_image(struct lcdinfo *LCD, const char *filename)
{
    // 1. 解码 PNG → RGB24
    png_image img;
    memset(&img, 0, sizeof(img));   // png_image 使用前必须先清零
    img.version = PNG_IMAGE_VERSION;

    if(!png_image_begin_read_from_file(&img, filename))
    {
        fprintf(stderr, "open png failed: %s\n", img.message);
        return;
    }

    img.format = PNG_FORMAT_RGB;    // 解码输出:每像素 3 字节 (R,G,B)

    unsigned char *rgbdata = calloc(1, PNG_IMAGE_SIZE(img));
    if(!png_image_finish_read(&img, NULL, rgbdata, 0, NULL))
    {
        fprintf(stderr, "decode png failed: %s\n", img.message);
        free(rgbdata);
        return;
    }

    int png_w = img.width;          // 图片宽
    int png_h = img.height;         // 图片高

    // 2. RGB24 → BGRX32 写入显存(居中,超界截断)
    int dx = (LCD->width  - png_w) / 2;     // 水平居中偏移(可为负:图比屏宽)
    int dy = (LCD->height - png_h) / 2;     // 垂直居中偏移(可为负:图比屏高)

    for(int j=0; j<png_h; j++)              // 逐行处理
    {
        int sy = j + dy;                    // 图片第 j 行 → 屏幕第 sy 行
        if(sy < 0 || sy >= LCD->height)     // 超出屏幕的行,丢弃
            continue;

        unsigned char *src = rgbdata + j*png_w*3;   // 源:图片一行
        unsigned char *dst = LCD->fbmem + sy*LCD->pitch; // 目标:屏幕一行

        for(int i=0; i<png_w; i++)          // 逐像素
        {
            int sx = i + dx;                // 图片第 i 列 → 屏幕第 sx 列
            if(sx < 0 || sx >= LCD->width)  // 超出屏幕的列,丢弃
                continue;

            // LCD 是 32bpp BGRX:字节0=B, 1=G, 2=R, 3=无效
            dst[sx*4+0] = src[i*3+2];       // B ← 图片的 R
            dst[sx*4+1] = src[i*3+1];       // G ← 图片的 G
            dst[sx*4+2] = src[i*3+0];       // R ← 图片的 B
            dst[sx*4+3] = 0;                // X 字节不参与显示
        }
    }

    // 3. 释放解码数据
    free(rgbdata);
}
