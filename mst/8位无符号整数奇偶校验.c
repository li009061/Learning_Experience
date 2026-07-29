// 编写一个简单的 C 语言程序，实现对一个 8 位无符号整数的奇偶校验。
// 若该整数中 1 的个数为奇数，则返回 1；若为偶数，则返回 0。

#include <stdio.h>
#include <stdint.h>

char result[256];

void odd_even(void)
{
    for(int k=0; k<256; k++)
    {
        int count = 0;
        for(int i=0; i<8; i++)
        {
            if(k & (0x01<<i))
                count++;
        }
        result[k] = count%2;
    }
}

int main(int argc, char const *argv[])
{
    // 1. 将所有的可能性都算好
    odd_even();

    // 2. 直接查表
    uint8_t n = 7;
    printf("%d\n", result[n]);

    return 0;
}
