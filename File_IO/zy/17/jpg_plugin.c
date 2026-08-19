// libJPG.so 插件：显示 JPEG 图片（复用 15 题代码）
// 统一插件接口：int show_image(const char *filename)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <jpeglib.h>

struct imageJPG
{
	int width;
	int height;
	int pixel_size;
	int jpg_size;
	char *jpg;
};

// 加载JPEG图片：读文件字节到 p->jpg
static struct imageJPG *load_jpg(const char *jpgFileName){
	FILE *fp = fopen(jpgFileName, "rb");
	if(fp == NULL){
		perror("loadJPG");
		return NULL;
	}

	struct imageJPG *p = calloc(1, sizeof(struct imageJPG));
	if(p == NULL){
		perror("calloc");
		fclose(fp);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	p->jpg_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	p->jpg = malloc(p->jpg_size);
	if(p->jpg == NULL){
		perror("malloc");
		free(p);
		fclose(fp);
		return NULL;
	}
	fread(p->jpg, 1, p->jpg_size, fp);
	fclose(fp);

	return p;
}

// 解码JPG图片：填 p->width/height/pixel_size，返回 RGB 数据
static char *jpg2rgb(struct imageJPG *p){
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, (unsigned char *)p->jpg, p->jpg_size);

	if(!jpeg_read_header(&cinfo, true)){
		fprintf(stderr, "jpeg_read_header failed: %s\n", strerror(errno));
		return NULL;
	}
	jpeg_start_decompress(&cinfo);

	p->width = cinfo.output_width;
	p->height = cinfo.output_height;
	p->pixel_size = cinfo.output_components;

	int linesize = p->width * p->pixel_size;
	char *rgb = calloc(1, linesize * p->height);

	while(cinfo.output_scanline < p->height){
		unsigned char *row = (unsigned char *)rgb + cinfo.output_scanline * linesize;
		jpeg_read_scanlines(&cinfo, &row, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return rgb;
}

// 显示JPG图片：大图等比缩小居中，小图原尺寸居中
static void display_jpg(struct imageJPG *p){
	char *rgb = jpg2rgb(p);
	if(rgb == NULL)
		return;

	int lcd = open("/dev/fb0", O_RDWR);
	if(lcd < 0){
		perror("open fb0");
		free(rgb);
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
		free(rgb);
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

	for(int y = 0; y < dh; y++){
		int sy = y * p->height / dh;
		for(int x = 0; x < dw; x++){
			int sx = x * p->width / dw;
			char *s = rgb + sy * p->width * p->pixel_size + sx * p->pixel_size;
			char *d = fb + (y0 + y) * pitch + (x0 + x) * 4;
			d[0] = s[2]; d[1] = s[1]; d[2] = s[0];   // RGB888 -> XR24(BGR)
		}
	}

	munmap(fb, pitch * sh);
	close(lcd);
	free(rgb);
}

static void release_jpg(struct imageJPG *p){
	if(p){
		free(p->jpg);
		free(p);
	}
}

// 统一插件接口（唯一导出符号）
int show_image(const char *filename){
	struct imageJPG *p = load_jpg(filename);
	if(p == NULL)
		return -1;
	display_jpg(p);
	release_jpg(p);
	return 0;
}
