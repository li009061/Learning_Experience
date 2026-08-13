#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        printf("用法：%s <源文件> <目标文件>\n", argv[0]);
        return -1;
    }

    // 打开源文件（fd称为文件句柄，handle）
    int fd1 = open(argv[1], O_RDONLY); // 0 1 2 3 4 5 ....
    if(fd1 == -1)
    {
        perror("打开源文件失败");
        return 0;
    }
    
    // 打开目标文件
    //------------------------ 读        创建     覆盖    权限
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd2 == -1)
    {
        perror("打开目标文件失败");
        return 0;
    }


    char buf[1024];
    while(1)
    {
        int n = read(fd1, buf, sizeof(buf));  // n <= 1024

        if(n == -1)
        {
            perror("读取文件失败");
            break;
        }

        if(n == 0) // 读完了
            break;

        // 试图将n个字节写入文件fd2中
        // 但实际的入d字节数是m
        char *p = buf;  //----数组不可直接加减转换成指针
        while(n > 0){
            int m = write(fd2, p, n); // m <= n
            if( m == -1){
                perror("写入文件失败");
                break;
            }
            p += m;      //------没有读完将指针移到读到的位置
            n -= m;     //-----将剩下的读完
        }
    }

    // 关闭文件
    close(fd1);
    close(fd2);

    return 0;
}
