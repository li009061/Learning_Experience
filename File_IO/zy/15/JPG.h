#ifndef JPG_H
#define JPG_H


struct imageJPG
{
	int width;       // 解码后的图片宽（像素）
	int height;      // 解码后的图片高（像素）
	int pixel_size;  // 每像素字节数（RGB888=3）

	int jpg_size;    // JPEG 文件字节数（解码必需，jpeg_mem_src 需要长度）

	char *jpg;       // JPEG 文件原始数据
};
// 加载JPEG图片
struct imageJPG *loadJPG(const char *jpgFileName);

// 解码JPG图片
char *jpg2rgb(struct imageJPG *p);

// 显示JPG图片
void displayJPG(struct imageJPG *p);

// 释放资源
void releaseJPG(struct imageJPG *p);

#endif // JPG_H
