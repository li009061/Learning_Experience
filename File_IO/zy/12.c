// （LCD与触摸屏）
// 编写一个程序，在开发板LCD屏幕上显示一个按钮，支持单击并有按下、弹起的效果，如下图所示：

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>

#include "jpeglib.h"

int jpg_w;
int jpg_h;
int jpg_bpp; // 色深（单位是比特）
int jpg_pitch;

int jpg_on_w;
int jpg_on_h;
int jpg_on_bpp; // 色深（单位是比特）
int jpg_on_pitch;

int jpg_off_w;
int jpg_off_h;
int jpg_off_bpp; // 色深（单位是比特）
int jpg_off_pitch;

int lcd_w;
int lcd_h;
int lcd_bpp;
int lcd_pitch;
int lcd_size;

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

bool click(int touch){
    //获得触摸屏参数
    struct input_event touch_info;

    while(1){
        //清空结构体
        bzero(&touch_info, sizeof(touch_info));

        int n = read(touch, &touch_info, sizeof(touch_info));
        if(n < 0){
            perror("read touch_screen failed");
            return false;
        }

        if(touch_info.type == EV_KEY && touch_info.code == BTN_TOUCH){
            if(touch_info.value > 0)
                return true;
            else
                return false;
        }
    }
}

void turn_on(char *p, char *on_data){
    int x0 = (lcd_w - jpg_w) / 2;
    int y0 = (lcd_h - jpg_h) / 2;
    int i, j;
    for(i = 0; i < jpg_h; i++){                    
        for(j = 0; j < jpg_w; j++){                
            char *src = on_data + i*jpg_pitch + j*3;
            char pixel[3];
            memcpy(pixel, src, 3);
            char tmp = pixel[0]; pixel[0] = pixel[2]; pixel[2] = tmp;
            memcpy(p + (y0+i)*lcd_pitch + (x0+j)*4, pixel, 3);
        }
    }
}

void turn_off(char *p, char *off_data){
    int x0 = (lcd_w - jpg_w) / 2;
    int y0 = (lcd_h - jpg_h) / 2;
    int i, j;
    for(i = 0; i < jpg_h; i++){                    
        for(j = 0; j < jpg_w; j++){                
            char *src = off_data + i*jpg_pitch + j*3;
            char pixel[3];
            memcpy(pixel, src, 3);
            char tmp = pixel[0]; pixel[0] = pixel[2]; pixel[2] = tmp;
            // 拷到居中位置（行 y0+i、列 x0+j）
            memcpy(p + (y0+i)*lcd_pitch + (x0+j)*4, pixel, 3);      
        }
    }

}

int main(int argc, char const *argv[])
{
    //打开触摸屏
    int touch = open("/dev/input/event6", O_RDONLY);
    if(touch < 0){
        fprintf(stderr, "open touchscreen failed");
        return -1;
    }

    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd < 0){
        fprintf(stderr, "open lcd failed");
        return -1;
    }

    //获取屏幕参数放在结构体中
    struct fb_var_screeninfo lcd_info;
    bzero(&lcd_info, sizeof(lcd_info));
    ioctl(lcd, FBIOGET_VSCREENINFO, &lcd_info);

    //获取长宽高
    lcd_w = lcd_info.xres;
    lcd_h = lcd_info.yres;
    lcd_bpp = lcd_info.bits_per_pixel;
    lcd_pitch = lcd_w * (lcd_bpp/8);
    lcd_size = lcd_pitch * lcd_h;

    //映射内存
    char *p = mmap(NULL, lcd_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED){
        perror("mmap failed");
        return -1;
    }

    //清屏
    memset(p, 0xFF, lcd_size);

    //打开图片
    FILE *on = fopen("on.jpg", "rb");
    if(on == NULL){
        fprintf(stderr, "open on.jpg failed");
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }

    FILE *off = fopen("off.jpg", "rb");
    if(off == NULL){
        fprintf(stderr, "open off.jpg failed");
        fclose(on);
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }

    //获取jepg的图片大小
    fseek(on, 0, SEEK_END);
    int on_size = ftell(on);
    fseek(on, 0, SEEK_SET);

    fseek(off, 0, SEEK_END);
    int off_size = ftell(off);
    fseek(off, 0, SEEK_SET);

    //设置存放图片数据的堆
    char *on_data = calloc(1, on_size);
    if(on_data == NULL){
        perror("calloc on_data failed");
        fclose(on);
        fclose(off);
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }

    char *off_data = calloc(1, off_size);
    if(off_data == NULL){
        perror("calloc off_data failed");
        free(on_data);
        fclose(on);
        fclose(off);
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }

    //读取JPEG文件内容到缓冲区
    fread(on_data, 1, on_size, on);
    fread(off_data, 1, off_size, off);

    // 将JPEG图片解码为RGB数据放入堆中
    char * on_info = jpg2rgb(on_data, on_size);
    if (on_info == NULL) {
        fprintf(stderr, "decode on.jpg failed\n");
        free(on_data);
        free(off_data);
        fclose(on);
        fclose(off);
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }
    jpg_on_w = jpg_w;
    jpg_on_h = jpg_h;
    jpg_on_bpp = jpg_bpp;
    jpg_on_pitch = jpg_pitch;

    char * off_info = jpg2rgb(off_data, off_size);
    if (off_info == NULL) {
        fprintf(stderr, "decode off.jpg failed\n");
        free(on_data);
        free(off_data);
        fclose(on);
        fclose(off);
        munmap(p, lcd_size);
        close(lcd);
        close(touch);
        return -1;
    }
    jpg_off_w = jpg_w;
    jpg_off_h = jpg_h;
    jpg_off_bpp = jpg_bpp;
    jpg_off_pitch = jpg_pitch;

    fclose(on);
    fclose(off);

    while(1){

        bool control = click(touch);

        if(control){
            turn_on(p, on_info);
        }else
            turn_off(p, off_info);

    }

    //释放资源
    close(touch);
    close(lcd);
    munmap(p, lcd_size);
    free(on_data);
    free(off_data);
    free(on_info);
    free(off_info);
    return 0;
}

