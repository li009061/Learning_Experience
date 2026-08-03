/*
 * Linux 串口 RGB 控制程序
 * 默认端口: /dev/ttyUSB0 （根据你的系统改成 /dev/ttyACM0 等）
 * 波特率: 1500000
 * 功能: 每隔 1 秒切换一次屏幕颜色
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE   B1500000

static const unsigned char colors[][3] = {
    {255, 0,   0},   // 红
    {0,   255, 0},   // 绿
    {0,   0,   255}, // 蓝
    {255, 255, 0},   // 黄
    {0,   255, 255}, // 青
    {255, 0,   255}, // 紫
    {255, 255, 255}, // 白
    {0,   0,   0},   // 黑
};
static const int color_count = sizeof(colors) / sizeof(colors[0]);

static int open_serial(const char *port, speed_t baud)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("open");
        fprintf(stderr, "无法打开串口 %s\n", port);
        return -1;
    }

    /* 清空文件状态标志，改为阻塞模式 */
    fcntl(fd, F_SETFL, 0);

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, baud);
    cfsetispeed(&tty, baud);

    tty.c_cflag |= (CLOCAL | CREAD);    /* 忽略调制解调器状态，启用接收 */
    tty.c_cflag &= ~PARENB;             /* 无校验 */
    tty.c_cflag &= ~CSTOPB;             /* 1 位停止位 */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                 /* 8 位数据 */
    tty.c_cflag &= ~CRTSCTS;            /* 无硬件流控 */

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* 原始输入 */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         /* 无软件流控 */
    tty.c_oflag &= ~OPOST;                          /* 原始输出 */

    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    printf("串口 %s 已打开，波特率 1500000\n", port);
    return fd;
}

static int serial_write(int fd, const unsigned char *data, size_t len)
{
    ssize_t n = write(fd, data, len);
    if (n < 0) {
        perror("write");
        return -1;
    }
    printf("发送 (%zd 字节): ", n);
    for (size_t i = 0; i < (size_t)n; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return (int)n;
}

static void send_rgb(int fd, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char buf[64];
    int len = 0;
    int format = 0;  /* 0:#RRGGBB  1:R=xxx G=xxx B=xxx  2:二进制 0x55 0xAA R G B */

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

    serial_write(fd, buf, (size_t)len);
}

int main(void)
{
    int fd = open_serial(SERIAL_PORT, BAUD_RATE);
    if (fd < 0) {
        return 1;
    }

    printf("开始循环切换颜色，按 Ctrl+C 停止...\n");
    int idx = 0;
    while (1) {
        const unsigned char *c = colors[idx % color_count];
        send_rgb(fd, c[0], c[1], c[2]);
        idx++;
        usleep(1000000);  /* 1 秒 */
    }

    close(fd);
    return 0;
}
