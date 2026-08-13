#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char const *argv[])
{
    float n;
    // scanf("%d", &n); // 从键盘输入     // <---- read(0)
    // printf("n:%d\n", n); // 从终端输出 // ----> write(1)

    read(0, &n, 4);
    write(1, &n, 4);
    write(2, &n, 4);

    open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(3, &n, 4);

    return 0;
}
