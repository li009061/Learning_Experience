#ifndef __LCD_H__
#define __LCD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>

//lcd结构体
typedef struct lcd_info{

    int lcd_fd;

    int lcd_w;
    int lcd_h;
    int lcd_bpp;
    int lcd_pitch;
    int screensize;

    char *fbmem;
}lcd;

//lcd初始化
bool lcd_init(const char *dev_name, lcd *p);
//lcd资源释放
void lcd_release(lcd *p);


#endif // __LCD_H__