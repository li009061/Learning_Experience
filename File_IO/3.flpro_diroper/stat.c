// 标准输入输出头文件，用于printf打印信息
#include <stdio.h>
// 字符串操作头文件，提供bzero清零函数
#include <string.h>
// Unix标准库，提供系统调用相关功能
#include <unistd.h>
// 标准库，提供exit退出函数
#include <stdlib.h>
// 文件状态结构体stat、文件类型宏(S_IFMT/S_IFREG等)都在此头文件
#include <sys/stat.h>

/**
 * main函数：通过传入的文件路径获取文件元信息并判断文件类型
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组，argv[0]是程序名，argv[1]待检测文件路径
 * @return int 程序退出码，0正常结束，1参数错误
 */
int main(int argc, char const *argv[])
{
    // 判断命令行参数数量是否为2（程序名 + 文件路径）
    if(argc != 2)
    {
        // 参数格式提示：打印正确使用方式
        printf("Usage: %s <file>\n", argv[0]);
        // 参数非法，退出程序，状态码1标识异常退出
        exit(1);
    }

    // 定义stat结构体变量，用于存放stat系统调用获取的文件所有属性信息
    struct stat buf;
    // 将buf结构体内存全部清零，避免脏数据干扰结果
    bzero(&buf, sizeof(buf));

    // stat系统调用：根据传入路径argv[1]读取文件属性，存入buf结构体
    stat(argv[1], &buf);

    // 打印文件字节大小 st_size：stat结构体里代表文件大小的成员
    printf("size: %d\n", buf.st_size);
    // 打印文件inode编号 st_ino：stat结构体里代表文件索引节点号的成员
    printf("ino : %d\n", buf.st_ino);

    /*
     st_mode：文件权限+类型的组合数值
     S_IFMT：位掩码常量，按位与运算后可以从st_mode中单独提取出【文件类型】信息
    */
    switch(buf.st_mode & S_IFMT)
    {
        // S_IFSOCK：套接字文件类型，输出标识字符s
        case S_IFSOCK:
            printf("%c\n", 's');
            break; // 跳出switch，防止case穿透执行后续分支
        // S_IFLNK：符号链接(软链接)文件类型，输出标识字符l
        case S_IFLNK:
            printf("%c\n", 'l');
            break;
        // S_IFREG：普通常规文件类型，输出标识字符-
        case S_IFREG:
            printf("%c\n", '-');
            break;
        // S_IFBLK：块设备文件类型（如硬盘），输出标识字符b
        case S_IFBLK:
            printf("%c\n", 'b');
            break;
        // S_IFDIR：目录文件类型，输出标识字符d
        case S_IFDIR:
            printf("%c\n", 'd');
            break;
        // S_IFCHR：字符设备文件类型（如串口），输出标识字符c
        case S_IFCHR:
            printf("%c\n", 'c');
            break;
        // S_IFIFO：管道(FIFO)文件类型，输出标识字符p
        case S_IFIFO:
            printf("%c\n", 'p');
            break;
    }
    // 预留注释行：后续可扩展打印文件权限信息
    // printf("perm: %c\n");

    // 程序正常执行完毕，返回0
    return 0;
}
