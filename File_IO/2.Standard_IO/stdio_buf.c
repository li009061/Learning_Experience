#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    // 默认情况下，普通文件都是全缓冲
    FILE *fp = fopen("a.txt", "w+");

    // 数据会缓冲起来
    fputs("hello world\n", fp);

    // 1. 程序正常退出
    // return 0;

    // 2. 强制刷新数据到文件
    // fflush(fp);

    // 3. 关闭文件
    // fclose(fp);

    // 4. 填满缓冲区
    // int count = 0;
    // while(1)
    // {
    //     fputc('a', fp);

    //     // 偷偷查看一下文件的大小
    //     struct stat st;
    //     stat("a.txt", &st);
    //     if(st.st_size > 0)
    //         break;

    //     count++;
    // }
    // printf("count = %d\n", count);

    // 5. 读取文件
    // fread(NULL, 1, 1, fp);

    // 6. 改变缓冲区类型
    // setvbuf(fp, NULL, _IONBF, 0); // 无缓冲: 任何情况下都会刷新数据

    char buf[1024];
    setvbuf(fp, buf, _IOLBF, 1024); // 行缓冲: 除了全缓冲情况外，遇到\n也会刷新数据
    fputs("hello world", fp);

    abort();
}
