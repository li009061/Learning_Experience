#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>


// 读取触摸屏的轨迹坐标，并以(x,y)连续输出
int main(int argc, char const *argv[]){
    // 1. 打开触摸屏设备
    int tp = open("/dev/input/event6", O_RDONLY);
    if(tp < 0){
        fprintf(stderr, "打开/dev/input/event6失败\n");
        exit(1);    
    }

    // 2. 循环读取触摸屏数据    
    struct input_event ev;
    int x = 0, y = 0;
    while(1){
        bzero(&ev, sizeof(ev));

        int n = read(tp, &ev, sizeof(ev));
        if(n < 0){
            perror("读取文件失败!");
            return -1;
        }

        if(ev.type == EV_ABS && ev.code == ABS_X)
            x = ev.value;

        if(ev.type == EV_ABS && ev.code == ABS_Y)
            y = ev.value;

        if(ev.type == EV_SYN && ev.code == SYN_REPORT)
            printf("(%d,%d)\n", x, y);

        if(ev.type == EV_KEY && ev.code == BTN_TOUCH){
            if(ev.value == 1)
                printf("按下触摸屏\n");
            else
                printf("松开触摸屏\n");

        }
    }

}