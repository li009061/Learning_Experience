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

// 显示指定的一张bmp格式的图片

int main(int argc, char const *argv[]) // ./a.out xxx.bmp
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

    // 读取bmp文件RGB数据
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
        int n = read(fd, rgb, bmp_size); // n <= bmp_size
        if(n == -1)
        {
            perror("read bmp failed");
            return -1;
        }
        bmp_size -= n;
    }

    // 将RGB数据写入LCD
    int pixel = bmp_bpp/8;

    // 将整张BMP画上LCD
    for(int j=0; j<bmp_h && j<lcd_h; j++)
    {
        int lcd_offset = lcd_line_size*j;
        int bmp_offset = bmp_line_size*(bmp_h-j-1);

        // 将图片的一行画上LCD
        for(int i=0; i<bmp_w && i<lcd_w; i++)
        {
            memcpy(p+4*i + lcd_offset,
                   rgb+i*pixel + bmp_offset, pixel);
        }
    }

    // 释放资源
    munmap(p, lcd_size);
    close(lcd);
    close(fd);
    free(rgb);

    return 0;
}

/*
 * 程序总体步骤：
 * 1. 检查命令行参数，确保用户提供了要显示的 BMP 文件路径。
 * 2. 打开 LCD 帧缓冲设备 /dev/fb0，准备向屏幕写入数据。
 * 3. 通过 ioctl 获取 LCD 屏幕的可变参数（分辨率、色深等）。
 * 4. 使用 mmap 将 LCD 帧缓冲映射到用户空间，获得可直接操作的内存地址。
 * 5. 以只读方式打开用户指定的 BMP 文件。
 * 6. 读取 BMP 文件头 (bitmap_header) 和信息头 (bitmap_info)。
 * 7. 如果 BMP 使用了压缩格式，读取颜色表 (rgb_quad)。
 * 8. 根据 BMP 宽度、高度和色深计算每行的填充字节数和数据总大小。
 * 9. 申请内存并读取 BMP 的 RGB 像素数据。
 * 10. 将 BMP 像素逐行写入 LCD：由于 BMP 数据是从下往上存储的，
 *     因此需要从最后一行开始向上读取，再按从上到下的顺序显示到 LCD。
 * 11. 释放资源：解除内存映射、关闭文件描述符、释放动态内存。
 */