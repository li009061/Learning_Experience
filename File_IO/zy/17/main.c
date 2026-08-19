// 17 题：图片显示主程序（动态库参数化）
// 根据图片扩展名，自动 dlopen 对应的显示插件库并调用 show_image()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// 插件统一接口
typedef int (*show_fn)(const char *filename);

int main(int argc, char *argv[]){
	// 检查参数
	if(argc != 2){
		fprintf(stderr, "Usage: %s <image file>\n", argv[0]);
		return -1;
	}

	// 根据扩展名选择插件库
	const char *ext = strrchr(argv[1], '.');
	const char *lib;
	if(ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0))
		lib = "./libJPG.so";
	else if(ext && strcmp(ext, ".bmp") == 0)
		lib = "./libBMP.so";
	else{
		fprintf(stderr, "不支持的格式: %s\n", ext ? ext : "(无扩展名)");
		return -1;
	}

	// 加载插件（带 ./ 前缀 = 直接按路径找，不用配 LD_LIBRARY_PATH）
	void *handle = dlopen(lib, RTLD_NOW);
	if(handle == NULL){
		fprintf(stderr, "dlopen %s: %s\n", lib, dlerror());
		return -1;
	}

	// 取统一接口
	show_fn show = (show_fn)dlsym(handle, "show_image");
	if(show == NULL){
		fprintf(stderr, "dlsym show_image: %s\n", dlerror());
		return -1;
	}

	// 显示
	return show(argv[1]);
}
