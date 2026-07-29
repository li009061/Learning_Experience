//编写一个 C 语言函数，实现对一个 16 位无符号整数的位反转操作。例如，输入 0x1234，输出 0x2C48。
#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    uint16_t n = 0x1234; // 0001 0010 0011 0100
                         // 0010 1100 0100 1000 ==> m
    uint16_t m = 0;
    for(int i=0; i<16; i++)
    {
        m |= ((n&(0x0001<<i)) >>i ) << (15-i);
    }
    printf("%#hx\n", m);
    return 0;
}
