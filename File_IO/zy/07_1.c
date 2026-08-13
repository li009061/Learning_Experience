// 标准输入输出：printf/fprintf
#include <stdio.h>
// 字符串操作：bzero函数
#include <string.h>
// stat结构体、S_ISREG/S_ISDIR宏定义
#include <sys/stat.h>
// 系统文件操作相关（目录拷贝函数依赖）
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

// 函数前置声明（主函数内调用了这两个自定义函数，需提前声明）
// 普通文件拷贝函数
void copy_file(FILE *fp1,FILE *fp2);
// 目录递归拷贝函数
void copy_dir(const char *src_dir, const char *dst_dir);

int main(int argc, char const *argv[]) // 程序运行格式：./mycpy src dst
{
    // 判断命令行参数合法性：需要 程序名+源路径+目标路径 共3个参数
    if(argc != 3)
    {
        // 错误信息输出到标准错误流stderr
        fprintf(stderr, "Usage: %s src dst\n", argv[0]);
        return 1;
    }

    // 定义stat结构体，用于存储源文件元数据信息
    struct stat finfo;
    // 清空结构体内存，避免脏数据干扰
    bzero(&finfo, sizeof(finfo));
    // 通过系统调用stat获取源文件argv[1]的属性信息
    stat(argv[1], &finfo);

    // 判断源文件是否为普通文件，是则调用文件拷贝函数
    if(S_ISREG(finfo.st_mode)){
        FILE *pf1 = fopen(argv[1], "r");
        FILE *pf2 = fopen(argv[2], "w");
        copy_file(pf1, pf2);
        fclose(pf1);
        fclose(pf2);
    }

    // 判断源文件是否为目录，是则调用目录拷贝函数
    else if(S_ISDIR(finfo.st_mode))
        copy_dir(argv[1], argv[2]);

    // 其他文件类型不支持拷贝，打印错误提示
    else
        fprintf(stderr, "Error: 文件类型不支持\n");

    return 0;
}
