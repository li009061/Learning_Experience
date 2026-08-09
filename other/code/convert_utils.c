#include "convert_utils.h"


 //十进制转十六进制（不使用 %x）
void to_hex(unsigned int num, char *hex_str) {
    if (num == 0) {
        hex_str[0] = '0';
        hex_str[1] = '\0';
        return;
    }

    char temp[20]; 
    int index = 0;

    while (num > 0) {
        int remainder = num % 16;
        if (remainder < 10) {
            temp[index] = '0' + remainder;
        } else {
            temp[index] = 'A' + remainder - 10;
        }
        index++;
        num /= 16;
    }

    for (int i = 0; i < index; i++) {
        hex_str[i] = temp[index - 1 - i];
    }
    hex_str[index] = '\0';
}

//检测字节序（使用联合体或指针）
int is_little_endian(void) {
    unsigned int x = 1;
    unsigned char *p = (unsigned char *)&x;
    return (*p == 1) ? 1 : 0;
}

// 十进制转二进制字符串（调试用）
void to_binary(unsigned int num, int bits, char *bin_str) {
    for (int i = bits - 1; i >= 0; i--) {
        bin_str[bits - 1 - i] = ((num >> i) & 1) ? '1' : '0';
    }
    bin_str[bits] = '\0';
}