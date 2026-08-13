#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h> // for LCD
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 功能：每隔1秒，轮转显示RGB三原色
int main(int argc, char const *argv[])
{
    // 打开LCD帧缓冲设备
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1)
    {
        perror("open lcd failed");
        return 1;
    }

    // 获得LCD设备的硬件参数
    struct fb_var_screeninfo lcd_info;
    ioctl(lcd, FBIOGET_VSCREENINFO, &lcd_info);

    int lcd_w = lcd_info.xres;
    int lcd_h = lcd_info.yres;
    int lcd_bpp = lcd_info.bits_per_pixel; // 色深（单位是比特）
    int screen_size = lcd_w * lcd_h * lcd_bpp / 8;
    int lcd_line_size = lcd_w * lcd_bpp / 8;

    printf("LCD分辨率: %dx%d\n", lcd_w, lcd_h);

    // 给LCD映射一块与LCD分辨率恰好一致的内存
    char *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE,
                                      MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap failed");
        return 1;
    }

    uint32_t colors[] = {0x00FF0000, 0x0000FF00, 0x000000FF};

    for(int i = 0; ; i++)
    {
        // 填充颜色到整个屏幕
        for(int j=0; j<lcd_w*lcd_h; j++)
            memcpy(p+4*j, &colors[i%3], 4);

        sleep(1);
    }

    return 0;
}
