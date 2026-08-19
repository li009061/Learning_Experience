// libBMP.so 插件：显示 BMP 图片（复用 16 题代码）
// 统一插件接口：int show_image(const char *filename)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

struct imageBMP
{
	int width;
	int height;
	int pixel_size;
	char *rgb;
};

// BMP 文件头结构（与你 bmp.c 作业一致，packed 对齐）
struct bitmap_header
{
	int16_t type;
	int32_t size;
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits;
} __attribute__((packed));

struct bitmap_info
{
	int32_t size;
	int32_t width;
	int32_t height;
	int16_t planes;
	int16_t bit_count;
	int32_t compression;
	int32_t size_img;
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
} __attribute__((packed));

// 加载BMP图片：解析头 + 提取像素到 p->rgb（紧凑存储）
static struct imageBMP *load_bmp(const char *bmpFileName){
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

	if(info.bit_count != 24){
		fprintf(stderr, "只支持24位BMP(当前%d位)\n", info.bit_count);
		close(fd);
		return NULL;
	}

	lseek(fd, header.offbits, SEEK_SET);

	int h = info.height;
	int top_down = 0;
	if(h < 0){
		h = -h;
		top_down = 1;
	}

	int pixel = info.bit_count / 8;
	int line = info.width * pixel;
	int file_line = (line + 3) & ~3;

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

	for(int i = 0; i < h; i++){
		int dst = top_down ? i : (h - i - 1);
		read(fd, p->rgb + dst * line, line);
		lseek(fd, file_line - line, SEEK_CUR);
	}

	close(fd);
	return p;
}

// 显示BMP图片：大图等比缩小居中，小图原尺寸居中
static void display_bmp(struct imageBMP *p){
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

	int dw = p->width, dh = p->height;
	if(dw > sw || dh > sh){
		double scale = (double)sw / dw;
		if((double)sh / dh < scale)
			scale = (double)sh / dh;
		dw = (int)(dw * scale);
		dh = (int)(dh * scale);
	}

	int x0 = (sw - dw) / 2;
	int y0 = (sh - dh) / 2;

	int line = p->width * p->pixel_size;
	for(int y = 0; y < dh; y++){
		int sy = y * p->height / dh;
		for(int x = 0; x < dw; x++){
			int sx = x * p->width / dw;
			char *s = p->rgb + sy * line + sx * p->pixel_size;
			char *d = fb + (y0 + y) * pitch + (x0 + x) * 4;
			d[0] = s[0]; d[1] = s[1]; d[2] = s[2];   // BMP 本就是 BGR，直接拷
		}
	}

	munmap(fb, pitch * sh);
	close(lcd);
}

static void release_bmp(struct imageBMP *p){
	if(p){
		free(p->rgb);
		free(p);
	}
}

// 统一插件接口（唯一导出符号）
int show_image(const char *filename){
	struct imageBMP *p = load_bmp(filename);
	if(p == NULL)
		return -1;
	display_bmp(p);
	release_bmp(p);
	return 0;
}
