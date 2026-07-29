// 1.	用 C 语言实现一个函数，将无符号短整型（uint16_t）的高低字节交换
// （例如：输入 0x1234，输出 0x3412）。要求：使用位操作，不调用库函数。

#include <stdio.h>
#include <stdint.h>

uint16_t swap(uint16_t n)
{
    // 方法1：清晰版
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
    
    // 方法2：简洁版（依赖 uint16_t 截断）
    // return (n >> 8) | (n << 8);
}

int main(void)
{
    uint16_t n;
    
    printf("请输入十六进制数: ");
    scanf("%hx", &n);
    
    printf("原值: 0x%04X\n", n);
    
    uint16_t m = swap(n);
    printf("交换: 0x%04X\n", m);
    
    return 0;
}

// #include <stdio.h>
// #include <stdint.h>

// uint16_t swap(uint16_t n)
// {
//     uint16_t m = 0;

//     m |= (n>>8); // m = 0x0012;
//     m |= (n&0x00FF) << 8;

//     return m;
// }

// int main(int argc, char const *argv[])
// {
//     uint16_t n;
//     scanf("%hx", &n);
//     printf("%#hx\n", n);

//     uint16_t m = swap(n);
//     printf("%#hx\n", m);

//     return 0;
// }
