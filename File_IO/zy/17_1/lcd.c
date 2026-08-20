#include "lcd.h"

//lcd初始化
extern bool lcd_init(const char *dev_name, lcd *p){
    // 0. 判断参数合法性
    if(dev_name == NULL || p == NULL){
        return false;
    }

    // 1. LCD设备资源的准备
    p->lcd_fd = open(dev_name, O_RDWR);
    if(p->lcd_fd < 0){
        perror("open lcd failed");
        return false;
    }

    // 2. 获取LCD硬件参数
    struct fb_var_screeninfo vinfo;
    bzero(&vinfo, sizeof(vinfo));
    if(ioctl(p->lcd_fd, FBIOGET_VSCREENINFO, &vinfo) == -1){
        perror("ioctl failed");
        close(p->lcd_fd);
        return false;
    }

    p->lcd_w = vinfo.xres;
    p->lcd_h = vinfo.yres;
    p->lcd_bpp = vinfo.bits_per_pixel;
    p->lcd_pitch = p->lcd_w * (p->lcd_bpp/8);
    p->screensize = p->lcd_pitch * p->lcd_h;
    
    // 3. 内存映射
    p->fbmem = mmap(NULL, p->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, p->lcd_fd, 0);
    if(p->fbmem == MAP_FAILED){
        perror("mmap failed");
        close(p->lcd_fd);
        return false;
    }

    // 4. 清屏
    memset(p->fbmem, 0, p->screensize);
    return true;
}



//lcd资源释放
extern void lcd_release(lcd *p){
    if(p == NULL){
        return;
    }
    close(p->lcd_fd);
    munmap(p->fbmem, p->screensize);
    return;
}