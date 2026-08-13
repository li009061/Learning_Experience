#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h> 
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1){
        perror("open() faile");
        exit(0);
    }

    // ioctl获取屏幕的分辨率、色深等信息
    struct fb_var_screeninfo info;
    ioctl(lcd, FBIOGET_VSCREENINFO, &info);

    // 计算LCD屏幕的字节数
    int lcd_w = info.xres;
    int lcd_h = info.yres;
    int lcd_bpp = info.bits_per_pixel;
    int lcd_bytes = lcd_w * lcd_h * lcd_bpp / 8;
    int lcd_line_bytes = lcd_w * lcd_bpp /  8;

    printf("LCD屏幕尺寸: %d × %d\n", lcd_w, lcd_h);
    printf("LCD屏幕色深: %d\n", lcd_bpp);

    // 映射内存
    char *p = mmap(NULL, lcd_bytes, PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == (void *)-1){           //p == MAP_FAILED
        perror("mmap failed");
        return -1;
    }
    
    // 定义一个红色的像素点
    uint32_t red = 0x00FF0000;

    for(int j = 0;j < lcd_h; j++){
        // 画一条红线
        for(int i = 0; i < lcd_w;i++){
            memcpy(p+4*i+j*lcd_line_bytes, &red, 4);
        }
    }

    // 解除映射
    munmap(p , lcd_bytes);
    return 0;
}