// 1、（LCD与触摸屏）
// 编写一个程序，使得点击一次屏幕，换一种随机颜色。
// 提示：将“单击”的动作封装成一个独立、健壮的函数接口。

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <time.h>
#include <stdint.h>


//单击的实现
bool Click(int tp){
    //循环读取触摸屏设备
    struct input_event ev;

    int x1 , y1;
    int x2 , y2;

    bool x1_flag = false;
    bool y1_flag = false;

    while(1){
        //将结构体清空
        bzero(&ev, sizeof(ev));

        //读取数据存入结构体
        int n = read(tp, &ev, sizeof(ev));
        if(n < 0){  
            perror("读取触摸屏失败!");
            return false;
        }

        //读取按下一瞬间的x1和y1的坐标
        if(ev.type == EV_ABS && ev.code == ABS_X && !x1_flag){
            x2 = x1 = ev.value;
            x1_flag = true;
        }
        if(ev.type == EV_ABS && ev.code == ABS_Y && !y1_flag){
            y2 = y1 = ev.value;
            y1_flag = true;
        }

        //确定x2和y2的坐标
        if(ev.type == EV_ABS && ev.code == ABS_X)
            x2 = ev.value;
        if(ev.type == EV_ABS && ev.code == ABS_Y)
            y2 = ev.value;

        //判断是否为单击动作
        if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0){
            if(abs(x2 - x1) <= 200 && abs(y2 - y1) <= 200)
                return true;
            else{
                x1_flag = false;
                y1_flag = false;
                continue;
            }
        }   
    }
    return false;
}

uint32_t random_color_32(){

    uint32_t r = rand() % 256;
    uint32_t g = rand() % 256;
    uint32_t b = rand() % 256;

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}


int main(int argc, char const *argv[]){
    //打开触摸屏设备
    int tp = open("/dev/input/event6", O_RDONLY);
    if(tp < 0){
        fprintf(stderr, "open touchscreen failed!");
        return -1;
    }

    //打开显示屏
    int fb = open("/dev/fb0", O_RDWR);
    if(fb < 0){
        fprintf(stderr,"open fb failed!");
        return -1;
    }

    //读取显示屏的信息
    struct fb_var_screeninfo lcd_info;
    bzero(&lcd_info, sizeof(lcd_info));
    ioctl(fb, FBIOGET_VSCREENINFO, &lcd_info);

    //获取屏幕参数
    int lcd_w = lcd_info.xres;
    int lcd_h = lcd_info.yres;
    int lcd_bpp = lcd_info.bits_per_pixel;
    int lcd_pich = lcd_w * (lcd_bpp/8);
    int screen_size = lcd_pich * lcd_h;


    //映射内存
    char *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if(p == MAP_FAILED){
        perror("mmap failed");
        return -1;
    }

    //清屏
    memset(p, 0xFF, lcd_w * lcd_h * lcd_bpp / 8);

    //设置随机数
    time_t t = time(NULL);
    srand(t);

    //打印图片
    int j, i;
    while(1){

        bool a = Click(tp);

        uint32_t b = random_color_32();

        if(a){
            for(i = 0; i < lcd_h; i++){
                for(j = 0; j < lcd_w; j++){
                    memcpy(p+4*j+i*lcd_pich, (char *)&b, 4);
                }
            }
        }else
            continue;
    }

    //释放资源
    close(tp);
    close(fb);
    munmap(p, screen_size);
    return 0;
}