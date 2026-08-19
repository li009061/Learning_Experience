bool Click(int tp)
{
    struct input_event ev;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool pressed = false;
    bool got_x = false;   // ✅ 本次触摸周期内是否收到过 X
    bool got_y = false;   // ✅ 本次触摸周期内是否收到过 Y

    while (running) {
        int n = read(tp, &ev, sizeof(ev));
        if (n <= 0) {
            if (n < 0) perror("read tp");
            return false;
        }

        if (ev.type == EV_ABS && ev.code == ABS_X) {
            x2 = ev.value;
            if (!got_x) {      // 只记录本次周期的第一个 X
                x1 = ev.value;
                got_x = true;
            }
        }
        if (ev.type == EV_ABS && ev.code == ABS_Y) {
            y2 = ev.value;
            if (!got_y) {
                y1 = ev.value;
                got_y = true;
            }
        }

        if (ev.type == EV_KEY && ev.code == BTN_TOUCH) {
            if (ev.value == 1) {
                pressed = true;
                got_x = false;   // ✅ 新周期开始，强制重置
                got_y = false;
            } else if (ev.value == 0 && pressed) {
                pressed = false;
                
                // ✅ 关键：松开时先检查两个坐标都收到了，再判断
                if (got_x && got_y) {
                    if (abs(x2 - x1) <= 200 && abs(y2 - y1) <= 200)
                        return true;
                }
                
                got_x = false;
                got_y = false;
            }
        }
    }
    return false;
}

uint32_t random_color_32(void)
{
    uint32_t r = rand() % 256;
    uint32_t g = rand() % 256;
    uint32_t b = rand() % 256;
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}


int main(int argc, char const *argv[])
{
    signal(SIGINT, sig_handler);   // Ctrl+C 优雅退出

    int tp = open("/dev/input/event6", O_RDONLY);
    if (tp < 0) {
        perror("open touchscreen");
        return -1;
    }

    int fb = open("/dev/fb0", O_RDWR);
    if (fb < 0) {
        perror("open fb");
        close(tp);
        return -1;
    }

    struct fb_var_screeninfo lcd_info;
    memset(&lcd_info, 0, sizeof(lcd_info));
    ioctl(fb, FBIOGET_VSCREENINFO, &lcd_info);

    int lcd_w = lcd_info.xres;
    int lcd_h = lcd_info.yres;
    int lcd_bpp = lcd_info.bits_per_pixel;
    int lcd_pitch = lcd_w * (lcd_bpp / 8);
    int screen_size = lcd_pitch * lcd_h;

    char *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        close(tp);
        close(fb);
        return -1;
    }

    /* 清屏为白色 */
    memset(p, 0xFF, screen_size);

    srand(time(NULL));

    while (running) {
        if (!Click(tp))     // 读取失败则退出
            break;

        uint32_t color = random_color_32();

        /* 根据实际色深填充（兼容 16bpp 和 32bpp） */
        for (int y = 0; y < lcd_h; y++) {
            for (int x = 0; x < lcd_w; x++) {
                char *px = p + y * lcd_pitch + x * (lcd_bpp / 8);
                if (lcd_bpp == 32) {
                    *(uint32_t *)px = color;
                } else if (lcd_bpp == 16) {
                    uint16_t r5 = ((color >> 16) & 0xFF) >> 3;
                    uint16_t g6 = ((color >> 8)  & 0xFF) >> 2;
                    uint16_t b5 = (color & 0xFF) >> 3;
                    *(uint16_t *)px = (r5 << 11) | (g6 << 5) | b5;
                }
            }
        }
    }

    /* 释放资源 */
    munmap(p, screen_size);
    close(tp);
    close(fb);
    return 0;
}