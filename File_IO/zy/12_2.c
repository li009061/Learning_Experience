// 12_2.c 优化版：LCD 按钮（按下/弹起），居中 + RGB->BGR 修正
// 相对 12.c 的优化：全局变量->结构体；turn_on/turn_off 重复->一个 draw；
//                  重复资源清理->进程退出内核自动回收；未用变量删除

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "jpeglib.h"

typedef struct { char *rgb; int w, h, pitch; } Image;

// 读入 jpg 文件并解码成 RGB888；失败返回空 Image（rgb==NULL）
static Image load_jpg(const char *path)
{
    Image img = {0};
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror(path); return img; }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *raw = malloc(size);
    fread(raw, 1, size, fp);
    fclose(fp);

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (unsigned char *)raw, size);
    if (!jpeg_read_header(&cinfo, true)) return img;
    jpeg_start_decompress(&cinfo);

    img.w = cinfo.output_width;
    img.h = cinfo.output_height;
    img.pitch = img.w * cinfo.output_components;
    img.rgb = malloc(img.pitch * img.h);
    while (cinfo.output_scanline < img.h) {
        unsigned char *row = (unsigned char *)img.rgb + cinfo.output_scanline * img.pitch;
        jpeg_read_scanlines(&cinfo, &row, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(raw);
    return img;
}

// 把图片画到屏幕中央；fb 是 XR24(32bpp)，源是 RGB888，需交换 R/B
static void draw(char *fb, int fbw, int fbh, int pitch, const Image *img)
{
    int x0 = (fbw - img->w) / 2;
    int y0 = (fbh - img->h) / 2;
    for (int i = 0; i < img->h; i++)
        for (int j = 0; j < img->w; j++) {
            char *s = img->rgb + i * img->pitch + j * 3;
            char *d = fb + (y0 + i) * pitch + (x0 + j) * 4;
            d[0] = s[2]; d[1] = s[1]; d[2] = s[0];   // RGB -> BGR
        }
}

// 阻塞等待触摸事件：按下返回 1，松开返回 0
static int touched(int tp)
{
    struct input_event ev;
    while (read(tp, &ev, sizeof(ev)) > 0)
        if (ev.type == EV_KEY && ev.code == BTN_TOUCH)
            return ev.value > 0;
    return 0;
}

int main(void)
{
    int tp = open("/dev/input/event6", O_RDONLY);
    int fb = open("/dev/fb0", O_RDWR);
    if (tp < 0 || fb < 0) return 1;

    struct fb_var_screeninfo v;
    ioctl(fb, FBIOGET_VSCREENINFO, &v);
    int pitch = v.xres * (v.bits_per_pixel / 8);

    char *fbp = mmap(NULL, pitch * v.yres, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (fbp == MAP_FAILED) return 1;
    memset(fbp, 0xFF, pitch * v.yres);

    Image on = load_jpg("on.jpg"), off = load_jpg("off.jpg");
    if (!on.rgb || !off.rgb) return 1;

    while (1)
        draw(fbp, v.xres, v.yres, pitch, touched(tp) ? &on : &off);
}
