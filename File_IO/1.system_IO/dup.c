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
    // // 打开文件 a.txt ，获得其文件描述符 fd1
    // // 此处 fd1 就代表了这个文件及其配套的系统资源
    // int fd1 = open("a.txt", O_RDWR);

    // // 复制文件描述符 fd1，默认得到最小未用的文件描述符
    // int fd2 = dup(fd1);
    // write(fd1, "abc", 3); // abc --> a.txt
    // write(fd2, "xyz", 3); // xyz --> a.txt

    // // 复制文件描述符 fd1，并指派为 100
    // int fd3 = dup2(fd1, 100);
    // write(fd3, "www", 3); // www --> a.txt
    // write(100, "zzz", 3); // zzz --> a.txt

    // close(fd1);
    // close(fd2);
    // close(fd3);
    // close(100);

    system("ls -l");//1s -1 -->1(普通信息) (终端)

    int fd1 = open("a.txt", O_RDWR);
    if(fd1 == -1){
        perror("open() faile");
        exit(0);
    }
    dup2(fd1, 1);  //---->将1（终端\标准输出）覆盖成fd1也就是a.txt

    // fcntl(fd1, F_DUPFD, 1);  //----->这行与下面一行是等效的
    // dup(fd1);
    system("ls -l"); // 1s -1 --> fd1 (a.txt)

}