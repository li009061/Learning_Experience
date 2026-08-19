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

int jpg_w;
int jpg_h;
int jpg_bpp; // 色深（单位是比特）
int jpg_pitch;

// 参数说明：
//   jpgdata: jpg图片数据
//   jpgsize: jpg图片大小
// 返回值说明：
//   成功：指向rgb数据的指针
//   失败：NULL
char *jpg2rgb(const char *jpgdata, size_t jpgsize)
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
    printf("宽：  %d\n", jpg_w = cinfo.output_width);
    printf("高：  %d\n", jpg_h = cinfo.output_height);
    printf("色深：%d\n", jpg_bpp = cinfo.output_components*8);
    jpg_pitch = cinfo.output_width * cinfo.output_components;

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

// 显示一张指定的JPEG图片到LCD上

int main(int argc, char const *argv[])
{
    // 检查参数
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <jpgfile>\n", argv[0]);
        return 1;
    }

   // 准备lcd资源
    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        perror("open lcd failed");
        exit(1);
    }

    // 获取屏幕参数
    struct fb_var_screeninfo lcd_info;
    ioctl(lcd_fd, FBIOGET_VSCREENINFO, &lcd_info);
    int lcd_width = lcd_info.xres;
    int lcd_height = lcd_info.yres;
    int lcd_bpp = lcd_info.bits_per_pixel;
    int lcd_pitch = lcd_width * lcd_bpp/8;

    // 获得LCD的映射内存
    unsigned char *lcd_mem = mmap(NULL, lcd_width * lcd_height * lcd_bpp / 8,
                                  PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (lcd_mem == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

    // 清屏（白色）
    memset(lcd_mem, 0xFF, lcd_width * lcd_height * lcd_bpp / 8);

    // 打开JPEG图片并读取数据
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen failed");
        exit(1);
    }

    
    // 将文件指针移到文件末尾，获取文件的总大小
    fseek(fp, 0, SEEK_END);
    // 记录JPEG文件大小到jpgsize，同时用n变量保存该值用于后续读取循环
    int jpgsize = ftell(fp), 
    n=jpgsize;
    // 将文件指针重新移回文件开头，准备从头读取JPEG数据
    fseek(fp, 0, SEEK_SET);

    // 读取JPEG图片数据
    char *jpgdata = malloc(jpgsize);
    int total = 0;
    while(n > 0)
    {
        int m = fread(jpgdata+total, 1, jpgsize, fp); // n<=jpgsize
        n -= m;
        total+=m;
    }

    // 将JPEG图片解码为RGB数据
    char *rgbdata = jpg2rgb(jpgdata, jpgsize);

    // 将RGB数据拷贝到LCD上
    for(int j=0; j<lcd_height && j<jpg_h; j++)
    {
        int lcd_offset = lcd_pitch * j;
        int rgb_offset = jpg_pitch * j;

        // 显示一行
        for(int i=0; i<lcd_width && i<jpg_w; i++)
        {
            memcpy(lcd_mem+4*i + lcd_offset,
                   rgbdata+3*i + rgb_offset, 3);
        }
    }

    // 释放资源
    munmap(lcd_mem, lcd_width * lcd_height * lcd_bpp / 8);
    close(lcd_fd);
    free(jpgdata);
    free(rgbdata);
    fclose(fp);

    return 0;
}

