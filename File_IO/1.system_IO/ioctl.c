#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define LEDOP 0x1

int main(void)
{
    // 打开一盏LED灯
    int led = open("/dev/Led", O_RDWR);

    // 通过命令字 LEDOP 及其携带的0/1参数，控制LED灯的亮灭
    // 此处，LEDOP 是底层开发者自定义的命令字
    ioctl(led, LEDOP, 1);
    ioctl(led, LEDOP, 0);

    // 打开一个摄像头
    int cam = open("/dev/video0", O_RDWR);

    // 通过命令字 VIDIC_STREAMON 及其携带参数 vtype 启动摄像头
    enum v4l2_buf_type vtype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(cam, VIDIOC_STREAMON, &vtype);
}