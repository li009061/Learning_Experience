// 编写一个 C 语言函数，将一个 8 位无符号整数转换为十六进制字符串

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    unsigned int n = 0x12345678;

    char hex[10]; // = {'1','2','3', ...};
    bzero(hex, 10);

    for(int i=0; i<8; i++)
    {
        hex[i] = ((n&(0xF0000000>>(4*i))) >> (4*(8-i-1))) + '0';
    }

    // printf("%#x", n); // n --> "0x12345678" --> 终端
    // snprintf(hex, 10, "%#x", n); // n --> "0x12345678" --> hex[]
    printf("十六进制字符串：\"%s\"\n", hex);

    return 0;
}
