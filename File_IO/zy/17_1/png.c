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

#include "png.h" 
#include "lcd.h"

void show_image(lcd *lcd, const char* filename){
    // 1. 解码 PNG → RGB24
    //声明结构体，清理
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    //读PNG文件头
    if(!png_image_begin_read_from_file(&image, filename)){
        fprintf(stderr,"png_image_begin_read_from_file failed:%s\n",image.message);
        return;
    }

    // 指定输出格式：我要 RGB24
    image.format = PNG_FORMAT_RGB;

    unsigned char *rgbdata = calloc(1, PNG_IMAGE_SIZE(image));
    if(!png_image_finish_read(&image, NULL, rgbdata, 0, NULL)){
        fprintf(stderr,"decode png fauked:%s\n", image.message);
        free(rgbdata);
        return;
    }

    //获得宽，长
    int png_w = image.width;          // 图片宽
    int png_h = image.height;         // 图片高

    // 2. RGB24 → BGRX32 写入显存(居中,超界截断)
    int dx = (lcd->lcd_w - png_w) / 2;  // 水平居中偏移(可为负:图比屏宽)
    int dy = (lcd->lcd_h - png_h) / 2;  // 垂直居中偏移(可为负:图比屏高)

    for(int j = 0; j < png_h; j++){ // 逐行处理
        int sy = j + dy;            // 图片第 j 行 → 屏幕第 sy 行
        if(sy < 0 || sy >= lcd->lcd_h)  // 超出屏幕的行,丢弃
            continue;

        unsigned char *src = rgbdata + j*png_w*3;           // 源:图片一行
        unsigned char *dst = lcd->fbmem + sy*lcd->lcd_pitch;    // 目标:屏幕一行
        
        for(int i = 0; i < png_w; i++){             // 逐像素
            int sx = i + dx;                        // 图片第 i 列 → 屏幕第 sx 列
            if(sx < 0 || sx >= lcd->lcd_w)          // 超出屏幕的列,丢弃
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
    png_image_free(&image);
}