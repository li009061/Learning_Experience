// 8.	C 语言同意一些令人震惊的结构，下面的结构是合法的吗，如果是它做些什么？
// int a = 5, b = 7, c;
// c = a+++b;


//合法，这个的本质是看谁得优先性和结合性更高，后缀 ++ 先结合，a+++b = (a++) + b = 5 + 7 = 12，然后 a 变成 6。
#include <stdio.h>

void f(int x, int y)
{
    printf("x: %d\n", x);
    printf("y: %d\n", y);
}

int main(int argc, char const *argv[])
{
    int a=5, b=7, c;

    c = a+++b;
    printf("a:%d\n", a); // 6
    printf("b:%d\n", b); // 7
    printf("c:%d\n", c); // 12

    int w = 100;
    f(w++, w++);  //---------->在gcc的编译环境下从右到左传参

    return 0;
}
