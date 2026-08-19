// 测试程序：用自报错封装库复制文件
#include <stdio.h>
#include <fcntl.h>
#include "fileio.h"

int main(int argc, char *argv[])
{
    if(argc != 3){
        printf("用法: %s <源文件> <目标文件>\n", argv[0]);
        return -1;
    }

    int src = fio_open(argv[1], O_RDONLY);
    int dst = fio_open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if(src < 0 || dst < 0)
        return -1;

    char buf[1024];
    ssize_t n;
    while((n = fio_read(src, buf, sizeof(buf))) > 0)
        fio_write(dst, buf, n);

    fio_close(src);
    fio_close(dst);
    printf("复制完成\n");
    return 0;
}
