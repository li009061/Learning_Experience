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

// 在LCD上居中显示指定的一张BMP格式图片
// 当图片尺寸小于LCD分辨率时，图片居中显示；大于或等于时，从左上角开始显示（裁剪）

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("usage: %s <bmp>\n", argv[0]);
        return -1;
    }

    // 准备LCD资源
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1)
    {
        perror("open lcd failed");
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    ioctl(lcd, FBIOGET_VSCREENINFO, &vinfo);

    int lcd_w = vinfo.xres;
    int lcd_h = vinfo.yres;
    int lcd_bpp = vinfo.bits_per_pixel;
    int lcd_line_size = lcd_w * lcd_bpp / 8;
    int lcd_size = lcd_w * lcd_h * lcd_bpp / 8;

    char *p = mmap(NULL, lcd_size, PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap lcd failed");
        return -1;
    }

    // 读取bmp文件格式头
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("open bmp failed");
        return -1;
    }

    struct bitmap_header header;
    struct bitmap_info   info;
    bzero(&header, sizeof(header));
    bzero(&info, sizeof(info));

    read(fd, &header, sizeof(header));
    read(fd, &info, sizeof(info));

    if(info.compression != 0)
    {
        struct rgb_quad quad;
        bzero(&quad, sizeof(quad));
        read(fd, &quad, sizeof(quad));
    }

    int bmp_w = info.width;
    int bmp_h = info.height;
    int bmp_bpp = info.bit_count;

    printf("图片分辨率:%d×%d\n", info.width, info.height);

    // 计算一行中的无效字节数
    int pad = (4 - (bmp_w*bmp_bpp/8 % 4)) % 4;
    int bmp_line_size = bmp_w * bmp_bpp / 8 + pad;
    int bmp_size = bmp_line_size * bmp_h;

    char *rgb = malloc(bmp_size);
    bzero(rgb, bmp_size);

    // 读取bmp文件RGB数据
    while(bmp_size > 0)
    {
        int n = read(fd, rgb, bmp_size);
        if(n == -1)
        {
            perror("read bmp failed");
            return -1;
        }
        bmp_size -= n;
    }

    // 清屏：将LCD全部像素置为黑色，避免居中显示时周围残留旧画面
    memset(p, 0, lcd_size);

    // 计算实际显示区域的宽度和高度（取BMP和LCD的较小值，防止越界）
    int display_w = (bmp_w < lcd_w) ? bmp_w : lcd_w;
    int display_h = (bmp_h < lcd_h) ? bmp_h : lcd_h;

    // 计算居中显示时的起始偏移量
    // 当图片小于屏幕时，偏移量为正，图片居中
    // 当图片大于或等于屏幕时，偏移量为0，从左上角开始显示
    int x_offset = (lcd_w - display_w) / 2;
    int y_offset = (lcd_h - display_h) / 2;

    int pixel = bmp_bpp/8;

    // 将BMP数据写入LCD
    // BMP图像数据是从下到上存储的，因此第j行对应BMP的倒数第j行
    for(int j = 0; j < display_h; j++)
    {
        // LCD目标行的偏移量：加上y_offset实现垂直居中
        int lcd_offset = lcd_line_size * (j + y_offset);
        // BMP源行的偏移量：从最后一行向上读取
        int bmp_offset = bmp_line_size * (bmp_h - j - 1);

        for(int i = 0; i < display_w; i++)
        {
            // LCD目标像素地址：x方向加上x_offset实现水平居中
            memcpy(p + 4*(i + x_offset) + lcd_offset,
                   rgb + i*pixel + bmp_offset, pixel);
        }
    }

    // 释放资源
    munmap(p, lcd_size);
    close(lcd);
    close(fd);
    free(rgb);

    return 0;
}
