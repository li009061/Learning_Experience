#include "gif_lib.h"
#include "lcd.h"

void show_image(lcd *lcd, const char *filename){
    // 1. 打开
    int err = 0;
    GifFileType *gif = DGifOpenFileName(filename, &err);
    if(gif == NULL){ 
        fprintf(stderr, "open gif failed: %d\n", err); 
        return; 
    }

    // 2. 读入所有帧
    if(DGifSlurp(gif) == GIF_ERROR){
        fprintf(stderr, "slurp failed\n"); 
        return; 
    }

    // 3. 取第一帧（先显示静态帧；多帧动画是加分项）
    SavedImage *img = &gif->SavedImages[0];
    int w = img->ImageDesc.Width;
    int h = img->ImageDesc.Height;

    // 4. 选调色板：局部优先，没有就用全局（一行三元表达式）
    ColorMapObject *cmap = img->ImageDesc.ColorMap ? img->ImageDesc.ColorMap : gif->SColorMap;
    if(cmap == NULL){
        fprintf(stderr, "no color map\n");
        DGifCloseFile(gif, &err);
        return;
    }
    // 5. 像素索引 → 查调色板 → RGB → BGR 写显存
    for(int j = 0; j < h && j < lcd->lcd_h; j++){
        for(int i = 0; i < w && i < lcd->lcd_w; i++){
            int idx = img->RasterBits[j*w + i];      // 这个像素的"编号"
            GifColorType c = cmap->Colors[idx];       // 查调色板 → {R,G,B}
            // 写 fbmem：BGRX 顺序（GIF 给的是 RGB，记得换序）
            char *dst = lcd->fbmem + i*4 + j*lcd->lcd_pitch;  // 注意你的坐标约定
            dst[0] = c.Blue;   // B
            dst[1] = c.Green;  // G
            dst[2] = c.Red;    // R
            dst[3] = 0;        // X
        }
    }

    // 6. 关闭
    DGifCloseFile(gif, &err);
}