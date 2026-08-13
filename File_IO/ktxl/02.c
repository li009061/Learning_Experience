// 编写一个程序，通过计算读取数据的数量，获取指定文件的大小。

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("打开文件失败");
        return 1;
    }

    char buf[1024];
    long long total = 0;   // 用大类型防止溢出
    ssize_t n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        total += n;
    }

    if (n == -1) {
        perror("读取文件失败");
        close(fd);
        return 1;
    }

    printf("文件大小: %lld 字节\n", total);
    close(fd);
    return 0;
}
