#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

typedef enum {
    IMG_UNKNOWN,  // 默认值，通常为 0,未知图片格式
    IMG_BMP,
    IMG_JPEG,
    IMG_PNG,
    IMG_GIF
} ImageFormat;

ImageFormat detect_image_format(const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        perror("fopen failed");
        return IMG_UNKNOWN;
    }

    unsigned char header[12] = {0};
    fread(header, 1, 12, fp);
    fclose(fp);

    // 1. 检查 BMP：前 2 字节是 'BM'
    if (header[0] == 0x42 && header[1] == 0x4D) {
        return IMG_BMP;
    }

    // 2. 检查 JPEG：前 3 字节是 FF D8 FF
    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return IMG_JPEG;
    }

    // 3. 检查 PNG：固定 8 字节签名
    const unsigned char png_sig[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    if (memcmp(header, png_sig, 8) == 0) {
        return IMG_PNG;
    }

    // 4. 检查 GIF：前 3 字节是 'GIF'
    if (header[0] == 'G' && header[1] == 'I' && header[2] == 'F') {
        return IMG_GIF;
    }

    return IMG_UNKNOWN;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    // 1. 初始化LCD
    lcd *LCD = calloc(1, sizeof(lcd));
    if(!lcd_init("/dev/fb0", LCD)){
        perror("lcd_init failed");
        free(LCD);
        return -1;
    }

    printf("lcd_init success\n");
    // 2. 根据具体格式图片，选择不同的库函数来显示
    void *handler = NULL;
    void (*show_image)(lcd*, const char *) = NULL;

    switch(detect_image_format(argv[1]))
    {
    case IMG_JPEG: 
        handler = dlopen("libjpg.so", RTLD_LAZY);
        if(!handler){
            fprintf(stderr, "dlopen: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image = dlsym(handler, "show_image");
        if(!show_image){
            fprintf(stderr, "show_image: dlsym: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image(LCD, argv[1]);
        dlclose(handler);
        break;
    case IMG_BMP:
        handler = dlopen("libbmp.so", RTLD_LAZY);
        if(!handler){
            fprintf(stderr, "dlopen: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image = dlsym(handler, "show_image");
        if(!show_image){
            fprintf(stderr, "show_image: dlsym: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image(LCD, argv[1]);
        dlclose(handler);
        break;
    case IMG_PNG:
        handler = dlopen("libpngimg.so", RTLD_LAZY);
        if(!handler){
            fprintf(stderr, "dlopen: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image = dlsym(handler, "show_image");
        if(!show_image){
            fprintf(stderr, "show_image: dlsym: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image(LCD, argv[1]);
        dlclose(handler);
        break;
    case IMG_GIF:
        handler = dlopen("libgifimg.so", RTLD_LAZY);
        if(!handler){
            fprintf(stderr, "dlopen: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image = dlsym(handler, "show_image");
        if(!show_image){
            fprintf(stderr, "show_image: dlsym: %s\n", dlerror());
            free(LCD);
            return -1;
        }
        show_image(LCD, argv[1]);
        dlclose(handler);
        break;
    default:
        printf("Unknown image format\n");
        break;
    }

    // 3. 释放资源
    lcd_release(LCD);
    free(LCD);
    return 0;
}