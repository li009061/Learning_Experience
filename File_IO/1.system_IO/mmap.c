#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h> // for帧缓冲
#include <stdint.h>

int main(void)
{
    // // 以只读方式打开一个文件
    // int fd = open("a.txt", O_RDWR);

    // // 申请一块大小为1024字节的映射内存，并将之与文件fd相关联
    // char *p = mmap(NULL,  // 由系统自动分配
    //                1024,  // 映射内存大小
    //                PROT_READ|PROT_WRITE, // 映射内存的权限
    //                MAP_SHARED,  // 映射内存的共享属性
    //                fd, // 与映射内存相关联的文件描述符
    //                0); // 映射内存的偏移量

    // // 将该映射内存的内容打印出来（即其相关联文件fd的内容）
    // printf("%s\n", p);

    // // 通过操作内存，间接修改了文件内容
    // p[0] = 'x';
    // printf("%s\n", p);
    
    // // 解除映射
    // munmap(p, 1024);

    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd == -1)
    {
        perror("open lcd failed");
        return -1;
    }

    // ioctl获取屏幕的分辨率、色深等信息
    struct fb_var_screeninfo info;
    ioctl(lcd, FBIOGET_VSCREENINFO, &info);

    // 计算LCD屏幕的字节数
    int lcd_w = info.xres;
    int lcd_h = info.yres;
    int lcd_bpp = info.bits_per_pixel;
    int lcd_bytes = lcd_w * lcd_h * lcd_bpp / 8;
    int lcd_line_bytes = lcd_w * lcd_bpp / 8;

    printf("LCD屏幕尺寸: %d × %d\n", lcd_w, lcd_h);
    printf("LCD屏幕色深: %d\n", lcd_bpp);

    // 映射内存
    char *p = mmap(NULL, lcd_bytes, PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED)
    {
        perror("mmap failed");
        return -1;
    }

    // 定义一个红色的像素点
    uint32_t red = 0x00FF0000;

    for(int j=0; j<lcd_h; j++)
    {
        // 画一条红线
        for(int i=0; i<lcd_w; i++)
            memcpy(p+4*i + j*lcd_line_bytes, &red, 4);
    }

    // 解除映射
    munmap(p, lcd_bytes);

    return 0;
}