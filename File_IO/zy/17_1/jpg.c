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

#include "jpeglib.h"
#include "lcd.h"

typedef struct jpeg_info{

    int jpg_w;
    int jpg_h;
    int jpg_bpp;
    int jpg_pitch;

}jpg;

//分配内存，用来存放图片的尺寸信息
jpg jpg_info;

char *__jpg2rgb(const char *jpgdata, size_t jpgsize)
{
    // 1，声明解码结构体，以及错误管理结构体
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // 2，使用缺省的出错处理来初始化解码结构体
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // 3，配置该cinfo，使其从 jpgdata 中读取jpgsize个字节
    //    这些数据必须是完整的JPEG数据
    jpeg_mem_src(&cinfo, jpgdata, jpgsize);

    // 4，读取JPEG文件的头，并判断其格式是否合法
    if(!jpeg_read_header(&cinfo, true))
    {
        fprintf(stderr, "jpeg_read_header failed: "
            "%s\n", strerror(errno));
        return NULL;
    }

    // 5，开始解码
    jpeg_start_decompress(&cinfo);
    

    // 6，获取图片的尺寸信息
    printf("宽：  %d\n", jpg_info.jpg_w = cinfo.output_width);
    printf("高：  %d\n", jpg_info.jpg_h = cinfo.output_height);
    printf("色深：%d\n", jpg_info.jpg_bpp = cinfo.output_components*8);
    jpg_info.jpg_pitch = cinfo.output_width * cinfo.output_components;

    // 7，根据图片的尺寸大小，分配一块相应的内存rgbdata
    //    用来存放从jpgdata解码出来的图像数据
    unsigned long linesize = cinfo.output_width * cinfo.output_components;
    unsigned long rgbsize  = linesize * cinfo.output_height;
    char *rgbdata = calloc(1, rgbsize);

    // 8，循环地将图片的每一行读出并解码到rgb_buffer中
    int line = 0;
    while(cinfo.output_scanline < cinfo.output_height)
    {
        unsigned char *buffer_array[1];
        buffer_array[0] = rgbdata + cinfo.output_scanline * linesize;
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }

    // 9，解码完了，将jpeg相关的资源释放掉
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return rgbdata;
}

void show_image(lcd *lcd, const char *filename){
    if(lcd == NULL || filename == NULL){
        perror("show_image failed");
        return;
    }
    // 0. 获取文件数据
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        perror("fopen failed");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long jpgsize = ftell(fp), size = jpgsize;
    fseek(fp, 0, SEEK_SET);
    char *jpgdata = calloc(1, size);

    fread(jpgdata, 1, size, fp);

    fclose(fp);

    // 1. 解码JPG
    char *rgbdata = __jpg2rgb(jpgdata, jpgsize);
    if(rgbdata == NULL){
        perror("jpg2rgb failed");
        free(jpgdata);
        return;
    }
    // 2. 显示RGB
    for(int j = 0; j < lcd->lcd_h && j < jpg_info.jpg_h; j++){
        for(int i = 0; i < lcd->lcd_w && i < jpg_info.jpg_w; i++){
            // memcpy(lcd->fbmem+(j*lcd->lcd_pitch)+(i*jpg_info.jpg_bpp/8),
            //     rgbdata+(j*jpg_info.jpg_pitch)+(i*jpg_info.jpg_bpp/8),(jpg_info.jpg_bpp/8));
            memcpy(lcd->fbmem+(i*lcd->lcd_bpp/8)+(lcd->lcd_pitch*j+0),
                    rgbdata+(i*jpg_info.jpg_bpp/8)+(jpg_info.jpg_pitch*j+2),1);
            memcpy(lcd->fbmem+(i*lcd->lcd_bpp/8)+(lcd->lcd_pitch*j+1),
                    rgbdata+(i*jpg_info.jpg_bpp/8)+(jpg_info.jpg_pitch*j+1),1);
            memcpy(lcd->fbmem+(i*lcd->lcd_bpp/8)+(lcd->lcd_pitch*j+2),
                    rgbdata+(i*jpg_info.jpg_bpp/8)+(jpg_info.jpg_pitch*j+0),1);
        }
    }

    // 3. 释放资源
    free(jpgdata);
    return;
}


