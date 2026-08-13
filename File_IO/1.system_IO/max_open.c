#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    // int max = sysconf(_SC_OPEN_MAX);
    int max = 0;
    while(1)
    {
        int fd = open("a.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1)
        {
            perror("open");
            break;
        }
        max++;
    }

    printf("进程最多打开%d个文件\n", max);

    return 0;
}
