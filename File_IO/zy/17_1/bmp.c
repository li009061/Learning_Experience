#include "bmp.h"

void show_image(lcd *lcd, const char *filename){
    if(lcd == NULL || filename == NULL){
        perror("open bmp failed");
        return;
    }

    // 1. 准备BMP图片资源
    int bmp_fd = open(filename, O_RDONLY);
    if(bmp_fd < 0){
        perror("open bmp failed");
        return;
    }

    // 2. 获取BMP图片参数
    struct bitmap_header bmp_header;
    struct bitmap_info   bmp_info;
    bzero(&bmp_header, sizeof(bmp_header));
    bzero(&bmp_info, sizeof(bmp_info));
    read(bmp_fd, &bmp_header, sizeof(bmp_header));
    read(bmp_fd, &bmp_info, sizeof(bmp_info));

    //3. 查看是否需要调色板
    if(bmp_info.compression != 0){
        struct rgb_quad quad;
        bzero(&quad, sizeof(quad));
        read(bmp_fd, &quad, sizeof(quad));
    }

    int bmp_w = bmp_info.width;
    int bmp_h = bmp_info.height;
    int bmp_bpp = bmp_info.bit_count;

    // 4. 不含无效字节pad
    int bmp_pitch = bmp_w * (bmp_bpp/8);
    int pad = (4 - bmp_pitch % 4) % 4;
    int bmp_pitch_real = bmp_pitch + pad;
    int bmp_size = bmp_pitch * bmp_h;
    int bmp_size_real = bmp_pitch_real * bmp_h;

    // 5.读取BMP像素数据到内存
    char *bmp_data = calloc(1, bmp_size_real);
    if(bmp_data == NULL){
        perror("calloc failed");
        return;
    }

    read(bmp_fd, bmp_data, bmp_size_real);
    close(bmp_fd);

    // 6. 直接将bmp剩下的RGB数据读取到LCD的映射内存中
    int i,j;
    for(i = 0; i < lcd->lcd_h && i < bmp_h; i++){
        int lcd_offset = i             * lcd->lcd_pitch;
        int bmp_offset = (bmp_h -1 -i) * bmp_pitch_real;
        for(j = 0; j < lcd->lcd_w && j < bmp_w; j++){
            memcpy(lcd->fbmem + lcd_offset + j * (lcd->lcd_bpp/8),
                    bmp_data + bmp_offset + j * (bmp_bpp/8) , (bmp_bpp/8));
        }
    }
    free(bmp_data);
}