#include <stdio.h>
#include "convert_utils.h"

int main() {
    // ----- 测试 to_hex -----
    char hex[20];
    unsigned int test_nums[] = {0, 15, 16, 255, 1234, 0xABCDEF, 0xFFFFFFFF};
    int count = sizeof(test_nums) / sizeof(test_nums[0]);

    printf("===== 测试十进制转十六进制 =====\n");
    for (int i = 0; i < count; i++) {
        to_hex(test_nums[i], hex);
        printf("0x%X -> %s\n", test_nums[i], hex);
    }

    // ----- 测试字节序 -----
    printf("\n===== 测试字节序 =====\n");
    if (is_little_endian()) {
        printf("当前系统是小端序 (Little-Endian)\n");
    } else {
        printf("当前系统是大端序 (Big-Endian)\n");
    }

    // ----- 测试 to_binary -----
    char bin[33];
    printf("\n===== 测试十进制转二进制 =====\n");
    unsigned int bin_nums[] = {0, 1, 10, 255, 0xFF00, 0x12345678};
    int bcount = sizeof(bin_nums) / sizeof(bin_nums[0]);
    for (int i = 0; i < bcount; i++) {
        to_binary(bin_nums[i], 32, bin);
        printf("0x%X -> %s\n", bin_nums[i], bin);
    }

    printf("\n指定位数显示:\n");
    to_binary(10, 8, bin);
    printf("10 (8位) -> %s\n", bin);
    to_binary(255, 16, bin);
    printf("255 (16位) -> %s\n", bin);

    return 0;
}