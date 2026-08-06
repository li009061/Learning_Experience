#include "kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

struct node
{
    int a;
    int b;
    struct list_head list;
    int c;
};

int main(void)
{
    // 模拟 container_of 宏的原理：计算 list 成员在 struct node 中的偏移
    // 53453 是任意非零值，仅用于地址运算，不会真正访问内存
    printf("%ld\n", (long)&(((struct node *)53453)->list) - 53453);
    printf("%ld\n", (long)&(((struct node *)53453)->list) - 53453);
}