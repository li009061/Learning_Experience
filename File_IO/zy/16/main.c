#include "BMP.h"
#include <stdio.h>

int main(int argc, char const *argv[]){
	// 检查参数是否正确
	if(argc != 2){
		fprintf(stderr, "Usage: %s <bmpFileName>\n", argv[0]);
		return -1;
	}

	struct imageBMP *p = loadBMP(argv[1]);
	if(p == NULL)
		return -1;

	displayBMP(p);

	releaseBMP(p);

	return 0;
}
