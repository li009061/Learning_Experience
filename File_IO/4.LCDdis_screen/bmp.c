#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "bmp.h"


// 显示指定的一张bmp格式的图片(居中显示)
int main(int argc, char const *argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage: %s <bmp_flie>\n", argv[0]);
        return -1;
    }
    
    // 准备LCD资源
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1){
        perror("open lcd failed");
        return -1;
    }    
    
    // 定义结构体存储lcd分辨率
    struct fb_var_screeninfo vinfo;
    bzero(&vinfo, sizeof(vinfo));
    ioctl(lcd, FBIOGET_VSCREENINFO,&vinfo);

    // 提取lcd分辨率
    int lcd_w = vinfo.xres;
    int lcd_h = vinfo.yres;
    int lcd_bpp = vinfo.bits_per_pixel; 
    int lcd_line_size = lcd_w * (lcd_bpp/8);
    int lcd_size = lcd_w * lcd_h * (lcd_bpp/8);

    //映射lcd内存
    char *p = mmap(NULL, lcd_size, PROT_READ | PROT_WRITE, 
                    MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED){
        perror("mmap failed");
        return -1;
    }

    // 读取bmp文件格式头
    int fd = open(argv[1],O_RDONLY);
    if(fd == -1){
        perror("open bmp failed");
        return -1;
    }
    
    // 读取bmp文件RGB数据
    struct bitmap_header header;
    struct bitmap_info info;
    bzero(&header, sizeof(header));
    bzero(&info, sizeof(info));
    //将信息存储到header和info中
    read(fd, &header, sizeof(header));
    read(fd, &info, sizeof(info));

    //判断是否有调色板表
    if(info.compression != 0)
    {
        struct rgb_quad quad;
        bzero(&quad, sizeof(quad));
        read(fd, &quad, sizeof(quad));
    }

    // 提取bmp分辨率
    int bmp_w = info.width;
    int bmp_h = info.height;
    int bmp_bpp = info.bit_count;
    int bmp_line_size = bmp_w * (bmp_bpp / 8); 
    
    // 计算一行中的无效字节数
    int pad = (4 - (bmp_w * (bmp_bpp / 8) % 4)) % 4;
    bmp_line_size += pad;
    int bmp_size = bmp_line_size * bmp_h;
    
    char *rgb = malloc(bmp_size);
    if(rgb == NULL){
        perror("malloc failed");
        return -1;
    }
    bzero(rgb, bmp_size);

    // 读取bmp文件RGB数据(一次可能读不完,循环读)
    int remain = bmp_size;
    char *ptr = rgb;
    while(remain > 0){
        int n = read(fd, ptr, remain);
        if(n == -1){
            perror("read bmp failed");
            return -1;
        }
        remain -= n;
        ptr += n;
    }

    //居中后的位置
    int center_x, center_y;
    if(lcd_w >= bmp_w && lcd_h >= bmp_h){
        // 图片比屏幕小:上下左右都居中
        center_x = (lcd_w - bmp_w) / 2;
        center_y = (lcd_h - bmp_h) / 2;
    }
    else if(lcd_w >= bmp_w && lcd_h < bmp_h){
        // 宽够高不够:水平居中,垂直从顶部裁剪
        center_x = (lcd_w - bmp_w) / 2;
        center_y = 0;
    }
    else if(lcd_h >= bmp_h){
        // 高够宽不够:垂直居中,水平从左边裁剪
        center_x = 0;
        center_y = (lcd_h - bmp_h) / 2;
    }
    else{
        // 宽高都不够:从左上角裁剪
        center_x = 0;
        center_y = 0;
    }
    // 将BMP的像素数据逐行写入LCD
    int i, j;
    int pixel = bmp_bpp / 8;
    for(i = 0; i < lcd_h && i < bmp_h; i++){
        int lcd_offset = lcd_line_size * (i + center_y);
        int bmp_offset = bmp_line_size * (bmp_h - i - 1);
        for(j = 0; j < lcd_w && j < bmp_w; j++){
            memcpy(p + 4 * (j + center_x) + lcd_offset,
                    rgb + j * pixel + bmp_offset, pixel);
        }
    }
    
    // 释放资源
    munmap(p, lcd_size);
    close(lcd);
    close(fd);
    free(rgb);

    return 0;
}