#include "BMP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

// BMP 文件头结构（与你 bmp.c 作业一致，packed 对齐）
struct bitmap_header
{
	int16_t type;      // 'BM'
	int32_t size;      // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits;   // 像素数据偏移量
} __attribute__((packed));

struct bitmap_info
{
	int32_t size;      // 本结构大小
	int32_t width;     // 图像宽
	int32_t height;    // 图像高（负=自顶向下，正=自底向上）
	int16_t planes;
	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img;
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
} __attribute__((packed));

// 加载BMP图片：解析头 + 提取像素到 p->rgb（紧凑存储，去掉行填充字节）
struct imageBMP *loadBMP(const char *bmpFileName){
	int fd = open(bmpFileName, O_RDONLY);
	if(fd < 0){
		perror("loadBMP");
		return NULL;
	}

	struct bitmap_header header;
	struct bitmap_info info;
	bzero(&header, sizeof(header));
	bzero(&info, sizeof(info));
	read(fd, &header, sizeof(header));
	read(fd, &info, sizeof(info));

	// 只支持 24 位 BMP（调色板/其他色深复杂，暂不支持）
	if(info.bit_count != 24){
		fprintf(stderr, "只支持24位BMP(当前%d位)\n", info.bit_count);
		close(fd);
		return NULL;
	}

	// 直接定位到像素数据起始（自动跳过调色板）
	lseek(fd, header.offbits, SEEK_SET);

	// height 为负 = 自顶向下存储
	int h = info.height;
	int top_down = 0;
	if(h < 0){
		h = -h;
		top_down = 1;
	}

	int pixel = info.bit_count / 8;   // 每像素字节数
	int line = info.width * pixel;    // 一行像素字节数
	int file_line = (line + 3) & ~3;  // 文件里一行占的字节（4字节对齐）

	struct imageBMP *p = calloc(1, sizeof(struct imageBMP));
	if(p == NULL){
		perror("calloc");
		close(fd);
		return NULL;
	}

	p->width = info.width;
	p->height = h;
	p->pixel_size = pixel;
	p->rgb = malloc(line * h);
	if(p->rgb == NULL){
		perror("malloc");
		free(p);
		close(fd);
		return NULL;
	}

	// 逐行读取、跳过填充；正高度（自底向上）反转行序，
	// 使 p->rgb 统一为"自顶向下"，显示时无需再翻转
	for(int i = 0; i < h; i++){
		int dst = top_down ? i : (h - i - 1);
		read(fd, p->rgb + dst * line, line);
		lseek(fd, file_line - line, SEEK_CUR);   // 跳过填充字节
	}

	close(fd);
	return p;
}

// 显示BMP图片：大图等比缩小居中，小图原尺寸居中
void displayBMP(struct imageBMP *p){
	int lcd = open("/dev/fb0", O_RDWR);
	if(lcd < 0){
		perror("open fb0");
		return;
	}

	struct fb_var_screeninfo v;
	bzero(&v, sizeof(v));
	ioctl(lcd, FBIOGET_VSCREENINFO, &v);

	int sw = v.xres, sh = v.yres;
	int pitch = sw * (v.bits_per_pixel / 8);

	char *fb = mmap(NULL, pitch * sh, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
	if(fb == MAP_FAILED){
		perror("mmap");
		close(lcd);
		return;
	}

	// 等比缩放：比屏幕大则缩小，否则原尺寸
	int dw = p->width, dh = p->height;
	if(dw > sw || dh > sh){
		double scale = (double)sw / dw;
		if((double)sh / dh < scale)
			scale = (double)sh / dh;
		dw = (int)(dw * scale);
		dh = (int)(dh * scale);
	}

	// 居中偏移
	int x0 = (sw - dw) / 2;
	int y0 = (sh - dh) / 2;

	// 最近邻缩放 + 写屏
	// 关键：BMP 像素本就是 BGR 顺序，XR24 显存 byte0=B byte1=G byte2=R
	// —— 正好一致，直接拷，不需要像 JPEG(RGB) 那样交换！
	int line = p->width * p->pixel_size;
	for(int y = 0; y < dh; y++){
		int sy = y * p->height / dh;   // 源行
		for(int x = 0; x < dw; x++){
			int sx = x * p->width / dw;    // 源列
			char *s = p->rgb + sy * line + sx * p->pixel_size;
			char *d = fb + (y0 + y) * pitch + (x0 + x) * 4;
			d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
		}
	}

	munmap(fb, pitch * sh);
	close(lcd);
}

// 释放资源
void releaseBMP(struct imageBMP *p){
	if(p){
		free(p->rgb);
		free(p);
	}
}
