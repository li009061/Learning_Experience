#ifndef BMP_H
#define BMP_H
struct imageBMP
{
	int width;
	int height;
	int pixel_size;

	char *rgb;
};
// 加载BMP图片
struct imageBMP *loadBMP(const char *bmpFileName);

// 显示BMP图片
void displayBMP(struct imageBMP *p);

// 释放资源
void releaseBMP(struct imageBMP *p);


#endif // BMP_H