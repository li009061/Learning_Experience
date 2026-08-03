#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

/*
 * RGB 屏幕串口控制示例
 *
 * 环境：Linux
 * 默认串口：/dev/ttyUSB11（对应 Windows 上的 COM11）
 * 波特率：1500000
 *
 * 注意：Linux 下串口设备名通常是 /dev/ttyUSB* 或 /dev/ttyACM*，
 * 不一定是 /dev/ttyUSB11，请用 ls /dev/ttyUSB* 或 dmesg 确认实际设备。
 * 运行时可带参数指定设备，例如：./rgb_serial /dev/ttyUSB0
 */

#define DEFAULT_SERIAL  "/dev/ttyUSB11"
#define BAUD_RATE       1500000

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    const char *name;
} color_t;

static const color_t colors[] = {
    {255, 0,   0,   "红"},
    {0,   255, 0,   "绿"},
    {0,   0,   255, "蓝"},
    {255, 255, 0,   "黄"},
    {0,   255, 255, "青"},
    {255, 0,   255, "品红"},
    {255, 255, 255, "白"},
    {0,   0,   0,   "黑"},
};

static int serial_open(const char *dev)
{
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return -1;
    }

    /* 阻塞模式，便于 tcdrain 生效 */
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

    return fd;
}

static int serial_setup(int fd)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    /* 设置波特率 1500000（大多数现代 Linux 已支持） */
    if (cfsetispeed(&tty, B1500000) != 0 || cfsetospeed(&tty, B1500000) != 0) {
        fprintf(stderr, "警告：系统可能不支持 %d 波特率，请检查 termios 定义\n", BAUD_RATE);
        perror("cfsetspeed");
        return -1;
    }

    /* 8 位数据位、无校验、1 位停止位（8N1） */
    tty.c_cflag &= ~PARENB;          /* 无校验 */
    tty.c_cflag &= ~CSTOPB;          /* 1 位停止位 */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;              /* 8 位数据位 */
    tty.c_cflag |= CREAD | CLOCAL;   /* 启用接收，忽略调制解调器状态 */

    /* 原始输入输出，不处理特殊字符 */
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR);
    tty.c_oflag &= ~OPOST;

    /* 读超时：非阻塞，立即返回 */
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    return 0;
}

/*
 * 发送 RGB 颜色命令。
 * 当前使用示例协议：帧头 0xAA 0x55 + 命令 0x01 + R + G + B + 校验和 + 帧尾 0x55。
 * 实际使用时请根据你的屏幕/开发板通信协议修改此函数。
 */
static void send_rgb(int fd, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t frame[8];

    frame[0] = 0xAA;            /* 帧头高字节 */
    frame[1] = 0x55;            /* 帧头低字节 */
    frame[2] = 0x01;            /* 命令：设置 RGB */
    frame[3] = r;
    frame[4] = g;
    frame[5] = b;
    frame[6] = r + g + b;       /* 简单校验和 */
    frame[7] = 0x55;            /* 帧尾 */

    ssize_t n = write(fd, frame, sizeof(frame));
    if (n < 0) {
        perror("write");
    } else {
        tcdrain(fd);            /* 等待发送完成 */
    }
}

int main(int argc, char *argv[])
{
    const char *dev = (argc > 1) ? argv[1] : DEFAULT_SERIAL;

    int fd = serial_open(dev);
    if (fd < 0) {
        perror("open");
        fprintf(stderr, "无法打开串口 %s，请检查设备名和权限（可能需要 sudo 或 dialout 组）\n", dev);
        return 1;
    }

    if (serial_setup(fd) != 0) {
        close(fd);
        return 1;
    }

    printf("通过串口 %s 控制 RGB 屏幕，波特率 %d\n", dev, BAUD_RATE);
    printf("按 Ctrl+C 停止\n");

    size_t i = 0;
    size_t count = sizeof(colors) / sizeof(colors[0]);

    while (1) {
        const color_t *c = &colors[i % count];

        printf("[%zu] 切换到：%s (R=%d, G=%d, B=%d)\n",
               i, c->name, c->r, c->g, c->b);

        send_rgb(fd, c->r, c->g, c->b);

        sleep(1);
        i++;
    }

    close(fd);
    return 0;
}
