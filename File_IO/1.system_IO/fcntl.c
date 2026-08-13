#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    // system("ls -l");  // ls -l --> 1（普通信息）（终端）

    // int fd1 = open("a.txt", O_RDWR);
    // dup2(fd1, 1);


    // fcntl(fd1, F_DUPFD, 1);
    // dup(fd1);

    // system("ls -l");  // ls -l --> fd1（a.txt）

    // ...
    int fd[2];
    pipe(fd);  // fd[1] -->  管道  --->  fd[0]

    // 将管道文件为非阻塞状态
    long state = fcntl(fd[0], F_GETFL);
    state |= O_NONBLOCK;
    fcntl(fd[0], F_SETFL, O_NONBLOCK);

    char buf[3];
    if(read(fd[0], buf, 3) > 0)
        printf("%s\n", buf);
    else
        printf("读不到数据\n");

}