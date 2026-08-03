#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

/*
 * 串口 RGB 控制程序（Linux 版）
 * 默认设备: /dev/ttyS0（根据实际情况修改）
 * 波特率: 1500000
 * 功能: 每隔 1 秒切换一次屏幕颜色
 *
 * 发送格式可在 send_rgb() 中切换：
 *   0 -> 文本格式: "#RRGGBB\n"
 *   1 -> 文本格式: "R=xxx G=xxx B=xxx\n"
 *   2 -> 二进制格式: 0x55 0xAA R G B
 */

#define SERIAL_DEV   "/dev/ttyS1"
#define BAUD_RATE    B1500000       // 注意 termios 中 B1500000 需要系统支持

// 颜色表
static const unsigned char colors[][3] = {
    {255, 0,   0},
    {0,   255, 0},
    {0,   0,   255},
    {255, 255, 0},
    {0,   255, 255},
    {255, 0,   255},
    {255, 255, 255},
    {0,   0,   0},
};
static const int color_count = sizeof(colors) / sizeof(colors[0]);

// 打开串口
static int open_serial(const char *dev, speed_t baud)
{
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("打开串口失败");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // 设置波特率
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &options);

    tcflush(fd, TCOFLUSH);
    tcflush(fd, TCIFLUSH);

    printf("串口 %s 已打开，波特率 %d\n", dev, baud);
    return fd;
}

// 发送数据
static int serial_write(int fd, const unsigned char *data, int len)
{
    int written = write(fd, data, len);
    if (written < 0) {
        perror("写串口失败");
        return -1;
    }
    printf("发送 (%d 字节): ", written);
    for (int i = 0; i < written; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return written;
}

// 发送 RGB 颜色命令
static void send_rgb(int fd, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char buf[64];
    int len = 0;
    int format = 2;  // 0=#RRGGBB  1=R=xxx G=xxx B=xxx  2=二进制

    if (format == 0) {
        len = snprintf((char *)buf, sizeof(buf), "#%02X%02X%02X\n", r, g, b);
    } else if (format == 1) {
        len = snprintf((char *)buf, sizeof(buf), "R=%d G=%d B=%d\n", r, g, b);
    } else {
        buf[0] = 0x55;
        buf[1] = 0xAA;
        buf[2] = r;
        buf[3] = g;
        buf[4] = b;
        len = 5;
    }

    serial_write(fd, buf, len);
}

int main(void)
{
    int fd = open_serial(SERIAL_DEV, BAUD_RATE);
    if (fd < 0) {
        return 1;
    }

    int idx = 0;
    printf("开始循环切换颜色，按 Ctrl+C 停止...\n");

    while (1) {
        const unsigned char *c = colors[idx % color_count];
        send_rgb(fd, c[0], c[1], c[2]);
        idx++;
        sleep(1);
    }

    close(fd);
    return 0;
}