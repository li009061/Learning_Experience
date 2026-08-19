#include "JPG.h"

int main(int argc, char const *argv[]){
    // 检查参数是否正确
    if(argc != 2){
        fprintf(stderr, "Usage: %s <jpgFileName>\n", argv[0]);
        return -1;
    }

    struct imageJPG *p = loadJPG(argv[1]);
    if(p == NULL){
        return -1;
    }  
    // 解码JPG图片
    char *rgb = jpg2rgb(p);
    if(rgb == NULL){
        return -1;
    }

    displayJPG(p);

    releaseJPG(p);

    return 0;
}