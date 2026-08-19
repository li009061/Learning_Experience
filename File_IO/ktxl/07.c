// 编写一个程序，跟踪显示手指在触摸屏上滑动的轨迹坐标。
// 要求：
// 手指垂直于坐标轴滑动时，能正常输出坐标轨迹
// 手指松开触摸屏时，程序退出
// [root@GEC1808 /root]# ./touch
// (573, 221)
// (584, 217)
// (595, 240)
// (591, 246)
// ... ...


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

int main(int argc, char const *argv[])
{
    // 1. 打开触摸屏设备
    int tp = open("/dev/input/event6", O_RDONLY);
    if(tp < 0)
    {
        perror("打开/dev/input/event6失败");
        exit(1);
    }

    // 2. 循环读取触摸屏数据
    struct input_event ev;
    int x = 0, y = 0;
    while(1)
    {
        bzero(&ev, sizeof(ev));

        int n = read(tp, &ev, sizeof(ev));
        if(n < 0)
        {
            perror("读取触摸屏数据失败");
            exit(1);
        }

        if(ev.type == EV_ABS // 读取到绝对坐标事件类型
        && ev.code == ABS_X ) // 读取到x坐标
        {
            x = ev.value;
        }
        if(ev.type == EV_ABS // 读取到绝对坐标事件类型
        && ev.code == ABS_Y) // 读取到y坐标
        {
            y = ev.value;
        }

        // 3. 输出坐标
        if(ev.type == EV_SYN && ev.code == SYN_REPORT)
            printf("(%d,%d)\n", x, y);

        if(ev.type == EV_KEY     // 读取到按键事件类型
        && ev.code == BTN_TOUCH) // 读取到触摸事件
        {
            if(ev.value == 1)       // 按下了触摸屏
                printf("按下触摸屏\n");
            else{
                printf("松开触摸屏\n");
                break;
            }
            
        }

        // if(ev.type == EV_ABS        // 读取到绝对坐标事件类型
        // && ev.code == ABS_PRESSURE) // 读取到压力值
        // {
        //     if(ev.value == 1)       // 按下了触摸屏
        //         printf("按下触摸屏\n");
        //     else
        //         printf("松开触摸屏\n");
        // }
    }

    return 0;
}


















