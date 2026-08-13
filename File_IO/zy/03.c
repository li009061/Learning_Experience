// （标准IO缓冲区）
// 编写一个程序，检测标准IO的缓冲区的大小。

#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("标准IO缓冲区的大小为：%d\n", BUFSIZ);
    return 0;
}
