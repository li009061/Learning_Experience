#include <stdio.h>          // 提供 printf, perror
#include <stdlib.h>         // 提供 exit
#include <string.h>         // 提供 bzero（虽然推荐用 memset）
#include <sys/stat.h>       // 提供 struct stat, stat(), S_ISREG 等宏
#include <sys/sysmacros.h>  // 提供 major() 和 minor() 宏（用于分解设备号）
#include <unistd.h>         // 提供系统调用相关（本程序未直接使用，但保留）

int main(int argc, char *argv[])
{
    // 检查命令行参数个数，程序需要且仅需一个文件路径参数
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
        exit(1);
    }

    // 定义 struct stat 结构体变量，用于接收文件属性信息
    struct stat buf;

    // 将 buf 清零，避免残留数据影响后续读取
    // 虽然 stat() 会填充所有字段，但出于习惯我们仍然清空
    // 注意：bzero 是过时的函数，推荐使用 memset(&buf, 0, sizeof(buf))
    bzero(&buf, sizeof(buf));

    // 调用 stat() 获取指定文件的属性
    // 成功返回 0，失败返回 -1 并设置 errno
    if (stat(argv[1], &buf) == -1) {
        perror("获取文件属性失败");
        exit(1);
    }

    // 判断文件类型
    if (S_ISREG(buf.st_mode)) {
        // 如果是普通文件，打印该文件所在存储设备的设备号
        // st_dev 表示该文件所在的设备（即文件系统的设备号）
        // major/minor 分别提取设备号的主/次编号
        printf("该普通文件所在的设备号: %d, %d\n",
               major(buf.st_dev), minor(buf.st_dev));
    }
    else if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
        // 如果是字符设备或块设备文件，打印该设备文件本身的设备号
        // st_rdev 表示该设备文件所代表的实际设备的设备号
        printf("该设备文件的设备号: %d, %d\n",
               major(buf.st_rdev), minor(buf.st_rdev));
    }
    else {
        // 其他类型（如目录、符号链接、FIFO、套接字等）
        printf("该文件不是普通文件或设备文件，类型码: %o\n",
               buf.st_mode & S_IFMT);
    }

    return 0;
}