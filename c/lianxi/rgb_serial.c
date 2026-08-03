#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * 串口 RGB 控制程序
 * 端口: COM11
 * 波特率: 1500000
 * 功能: 每隔 1 秒切换一次屏幕颜色
 *
 * 发送格式可在 send_rgb() 中切换：
 *   0 -> 文本格式: "#RRGGBB\n"
 *   1 -> 文本格式: "R=xxx G=xxx B=xxx\n"
 *   2 -> 二进制格式: 0x55 0xAA R G B
 */

#define COM_PORT    "COM11"
#define BAUD_RATE   1500000

// 颜色表（红、绿、蓝、黄、青、紫、白、黑）
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

// 打开串口
static HANDLE open_serial(const char *port, DWORD baud)
{
    char full_name[32];
    snprintf(full_name, sizeof(full_name), "\\\\.\\%s", port);

    HANDLE h = CreateFileA(full_name,
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           0,
                           NULL);
    if (h == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "无法打开串口 %s\n", port);
        return INVALID_HANDLE_VALUE;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(h, &dcb)) {
        fprintf(stderr, "GetCommState 失败\n");
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }

    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(h, &dcb)) {
        fprintf(stderr, "SetCommState 失败，波特率 %lu 可能不支持\n", baud);
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 100;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 100;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(h, &timeouts);

    PurgeComm(h, PURGE_TXCLEAR | PURGE_RXCLEAR);
    printf("串口 %s 已打开，波特率 %lu\n", port, baud);
    return h;
}

// 发送数据
static int serial_write(HANDLE h, const unsigned char *data, DWORD len)
{
    DWORD written = 0;
    if (!WriteFile(h, data, len, &written, NULL)) {
        fprintf(stderr, "写串口失败\n");
        return -1;
    }
    printf("发送 (%lu 字节): ", written);
    for (DWORD i = 0; i < written; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
    return (int)written;
}

// 发送 RGB 颜色命令
static void send_rgb(HANDLE h, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char buf[64];
    int len = 0;
    int format = 0;  // 切换格式: 0=#RRGGBB  1=R=xxx G=xxx B=xxx  2=二进制

    if (format == 0) {
        // 文本格式: "#RRGGBB\n"
        len = snprintf((char *)buf, sizeof(buf), "#%02X%02X%02X\n", r, g, b);
    } else if (format == 1) {
        // 文本格式: "R=xxx G=xxx B=xxx\n"
        len = snprintf((char *)buf, sizeof(buf), "R=%d G=%d B=%d\n", r, g, b);
    } else {
        // 二进制格式: 0x55 0xAA R G B
        buf[0] = 0x55;
        buf[1] = 0xAA;
        buf[2] = r;
        buf[3] = g;
        buf[4] = b;
        len = 5;
    }

    serial_write(h, buf, (DWORD)len);
}

// 粗略延时 ms 毫秒
static void delay_ms(int ms)
{
    Sleep(ms);
}

int main(void)
{
    HANDLE h = open_serial(COM_PORT, BAUD_RATE);
    if (h == INVALID_HANDLE_VALUE) {
        return 1;
    }

    int idx = 0;
    printf("开始循环切换颜色，按 Ctrl+C 停止...\n");

    while (1) {
        const unsigned char *c = colors[idx % color_count];
        send_rgb(h, c[0], c[1], c[2]);
        idx++;
        delay_ms(1000);  // 1 秒
    }

    CloseHandle(h);
    return 0;
}
