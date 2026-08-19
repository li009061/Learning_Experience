// 3、（触摸屏操作）（选做）
// 编写一个画图软件，将开发板屏幕变成一个画板，用手指可以直接在开发板上画出任意线条，并可调节线条颜色。


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/input.h> // 系统定义输入设备操作的API

#include <pthread.h>
#include <semaphore.h>

#include "DRMwrap.h"

struct xy
{
    int x;
    int y;
};

struct drmHandle drm;
struct xy pos;
sem_t s;
int begin = 1;

int lcdw, lcdh, bpp;

void setPixel(struct xy pos, int color)
{
    memcpy(drm.vaddr + (pos.y*lcdw+pos.x)*bpp/8, &color, 4);
}

void drawline(struct xy pos1, struct xy pos2)
{
    int di = 0;

    int gx;
    int gy;
    gx = (pos1.x > pos1.x) ? 1 : -1;
    gy = (pos2.y > pos1.y) ? 1 : -1;

    int dx;
    int dy;
    dx = (pos2.x - pos1.x) * gx + 1;
    dy = (pos2.y - pos1.y) * gy + 1;

    int dc = 2;

    int color = 0x00FFFFFF;
    if(dx >= dy)
    {
        for(/*x2 += gx*/; pos1.x != pos2.x; pos1.x += gx)
        {
            di += dy;
            if(di <= dx)
            {
                setPixel(pos1, color);
                continue;
            }
            while((di > dx) && (pos1.y != pos2.y))
            {
                di -= dx;
                pos1.y += gy;
                setPixel(pos1, color);
            }
        }
    }
    else
    {
        for(/*y2 += gw*/; pos1.y != pos2.y; pos1.y += gy)
        {
            di += dx;
            if(di <= dy)
            {
                setPixel(pos1, color);
                continue;
            }
            while((di > dy) && (pos1.x != pos2.x))
            {
                di -= dy;
                pos1.x += gx;
                setPixel(pos1, color);
            }
        }
    }
}

void *drawLines(void *arg)
{
    struct xy pos1 = {0};
    struct xy pos2 = {0};

    while(1)
    {
        sem_wait(&s);
        pos1.x = pos.x;
        pos1.y = pos.y;

        while(1)
        {
            sem_wait(&s);

            if(begin == 1)
            {
                begin = 0;
                break;
            }

            // draw line
            pos2 = pos;

            drawline(pos1, pos2);

            // store postions
            pos1 = pos2;
        }
    }
}

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/dri/card0", O_RDWR);

    DRMinit(lcd);
    DRMcreateFB(lcd, &drm);
    DRMshowUp(lcd, &drm);

    lcdw = drm.width;
    lcdh = drm.height;
    bpp  = drm.pitch/drm.width*8;

    sem_init(&s, 0, 0);


    // draw lines
    pthread_t tid;
    pthread_create(&tid, NULL, drawLines, NULL);



    // get x and y
    int tp = open("/dev/input/event2", O_RDWR);
    struct input_event buf;
    while(1)
    {
        bzero(&buf, sizeof(buf));
        read(tp, &buf, sizeof(buf));

        if (buf.type == EV_ABS && buf.code == ABS_X)
        {
            pos.x = buf.value;
        }
        if (buf.type == EV_ABS && buf.code == ABS_Y)
        {
            pos.y = buf.value;
        }

        if (buf.type == EV_SYN)
        {
            sem_post(&s);
        }

        if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
        {
            begin = 1;
        }
    }

    return 0;
}