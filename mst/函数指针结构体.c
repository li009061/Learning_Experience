//C/C++：定义一个包含函数指针的结构体，并写完整示例代码演示使用方法。

#include <stdio.h>

struct node
{
    int a;
    float b;
    void (*show_info)(int,float);
};

void show_info(int a, float b)
{
    printf("%d, %f\n", a, b);
}

int main(int argc, char const *argv[])
{
    struct node n = {100, 3.14, show_info};
    n.show_info(n.a, n.b);
    return 0;
}
