#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main() {
    int a[5];
    char s[3];

    printf("a 的元素个数：%zu\n", sizeof(a) / sizeof(a[0]));
    printf("s 的元素个数：%zu\n", sizeof(s) / sizeof(s[0]));

    printf("%zu\n", ARRAY_SIZE(a));  // 输出 5
    return 0;
}