// 7.	Typedef 在 C 语言中频繁用以声明一个已经存在的数据类型的同义字，也可以用预处理器做类似的事。例如：
// #define dPS struct s *
// typedef struct s * tPS;
// 以上两种情况的意图都是要定义 dPS 和 tPS 作为一个指向结构 s 指针，哪种方法更好呢？（如果有的话）为什么？




// typedef 更好。 #define 是文本替换，dPS p1, p2 展开后 p2 不是指针；typedef 是类型定义，tPS p3, p4 两者都是指针。
#include <stdio.h>
struct s
{
    int a;
    char c;
}x;

#define dPS struct s *
typedef struct s * tPS; // 优先考虑

int main(int argc, char const *argv[])
{
    x.a = 100;
    x.c = 'w';

    struct s *p1 = &x;
    dPS        p2 = &x;
    tPS        p3 = &x;
    printf("a: %d, c: %c\n", p1->a, p1->c);
    printf("a: %d, c: %c\n", p2->a, p2->c);
    printf("a: %d, c: %c\n", p3->a, p3->c);

    dPS        pa, pb; // pa是指针，但pb是结构体
    struct s *pa, pb;

    tPS        pi, pj; // 这两个都是结构体指针

    return 0;
}
