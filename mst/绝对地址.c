// 在某工程中，要求设置一绝对地址为 0x67a9 的整型变量的值为 0xaa66。写代码实现该需求
#include<stdio.h>

int main(void)
{
    // 方法1：直接强制转换（简洁但不够安全）
    // *(int *)0x67a9 = 0xaa66;

    // 方法2：使用 volatile（推荐，防止编译器优化）
    volatile int *p = (volatile int *)0x67a9;
    *p = 0xaa66;

    // 方法3：使用宏定义（嵌入式常用）
    #define REG (*(volatile int *)0x67a9)
    REG = 0xaa66;

    return 0;
}