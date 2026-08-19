#include "JPG.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <jpeglib.h>


// 加载JPEG图片：读文件字节到 p->jpg
struct imageJPG *loadJPG(const char *jpgFileName){
    // 打开图片
    FILE *fp = fopen(jpgFileName, "rb");
    if(fp == NULL){
        perror("loadJPG");
        return NULL;
    }

    struct imageJPG *p = calloc(1, sizeof(struct imageJPG));
    if(p == NULL){
        perror("calloc");
        fclose(fp);
        return NULL;
    }

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    p->jpg_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 读入整个文件
    p->jpg = malloc(p->jpg_size);
    if(p->jpg == NULL){
        perror("malloc");
        free(p);
        fclose(fp);
        return NULL;
    }
    fread(p->jpg, 1, p->jpg_size, fp);
    fclose(fp);

    return p;
}

// 解码JPG图片：填 p->width/height/pixel_size，返回 RGB 数据（调用者负责释放）
char *jpg2rgb(struct imageJPG *p){
    // 1，声明解码结构体，以及错误管理结构体
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // 2，使用缺省的出错处理来初始化解码结构体
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // 3，配置该cinfo，使其从 p->jpg 中读取 p->jpg_size 个字节
    jpeg_mem_src(&cinfo, (unsigned char *)p->jpg, p->jpg_size);

    // 4，读取JPEG文件的头，并判断其格式是否合法
    if(!jpeg_read_header(&cinfo, true)){
        fprintf(stderr, "jpeg_read_header failed: %s\n", strerror(errno));
        return NULL;
    }

    // 5，开始解码
    jpeg_start_decompress(&cinfo);

    // 6，获取图片的尺寸信息
    p->width = cinfo.output_width;
    p->height = cinfo.output_height;
    p->pixel_size = cinfo.output_components;

    // 7，分配存放RGB数据的空间
    int linesize = p->width * p->pixel_size;
    char *rgb = calloc(1, linesize * p->height);

    // 8，循环地将图片的每一行读出并解码到rgb中
    while(cinfo.output_scanline < p->height){
        unsigned char *row = (unsigned char *)rgb + cinfo.output_scanline * linesize;
        jpeg_read_scanlines(&cinfo, &row, 1);
    }

    // 9，释放jpeg相关资源
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return rgb;
}

// 显示JPG图片：大图等比缩小居中，小图原尺寸居中
void displayJPG(struct imageJPG *p){
    char *rgb = jpg2rgb(p);
    if(rgb == NULL)
        return;

    // 打开LCD
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd < 0){
        perror("open fb0");
        free(rgb);
        return;
    }

    // 获取屏幕参数
    struct fb_var_screeninfo v;
    bzero(&v, sizeof(v));
    ioctl(lcd, FBIOGET_VSCREENINFO, &v);

    int sw = v.xres, sh = v.yres;
    int pitch = sw * (v.bits_per_pixel / 8);

    // 映射显存
    char *fb = mmap(NULL, pitch * sh, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if(fb == MAP_FAILED){
        perror("mmap");
        free(rgb);
        close(lcd);
        return;
    }

    // 计算显示尺寸：比屏幕大则等比缩小，否则原尺寸
    int dw = p->width, dh = p->height;
    if(dw > sw || dh > sh){
        double scale = (double)sw / dw;
        if((double)sh / dh < scale)
            scale = (double)sh / dh;
        dw = (int)(dw * scale);
        dh = (int)(dh * scale);
    }

    // 居中偏移
    int x0 = (sw - dw) / 2;
    int y0 = (sh - dh) / 2;

    // 最近邻缩放 + RGB->BGR + 写屏
    for(int y = 0; y < dh; y++){
        int sy = y * p->height / dh;   // 源图对应行
        for(int x = 0; x < dw; x++){
            int sx = x * p->width / dw;    // 源图对应列
            char *s = rgb + sy * p->width * p->pixel_size + sx * p->pixel_size;
            char *d = fb + (y0 + y) * pitch + (x0 + x) * 4;
            d[0] = s[2]; d[1] = s[1]; d[2] = s[0];   // RGB888 -> XR24(BGR)
        }
    }

    munmap(fb, pitch * sh);
    close(lcd);
    free(rgb);
}

// 释放资源
void releaseJPG(struct imageJPG *p){
    if(p){
        free(p->jpg);
        free(p);
    }
}
