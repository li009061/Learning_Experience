// 用所学知识，实现如下要求：
// 编写a.c，包含一个函数copyFile()，专门实现文件复制，并做成libcopyFile.a
// 编写b.c，包含一个函数copyDir()，专门实现目录复制，并做成libcopyDir.a
// 要求copyDir()通过调用copyFile()来实现。
// // 函数接口要求：
// // 参数fdSrc：源文件描述符
// // 参数fdDst：目标文件描述符
// // 返回值：成功复制的字节数，失败返回-1
// int copyFile(int fdSrc, int fdDst);

// // 函数接口要求：
// // 参数dirSrc：源目录名称
// // 参数dirDst：目标目录名称
// // 返回值：成功复制的文件数，失败返回-1
// int copyDir(const char *dirSrc, const char *dirDst);

// 编写一个主程序main.c，调用以上函数实现文件或目录的拷贝。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include "a.h"
#include "b.h"

int main(int argc, char const *argv[]){
    //判断参数是否正确
    if(argc != 3){
        printf("用法：%s <源文件/目录> <目标文件/目录>\n", argv[0]);
        return -1;
    }

    // 获取文件信息
    struct stat info;
    bzero(&info, sizeof(info));
    if(stat(argv[1], &info) == -1){
        perror("stat failed");
        return 1;
    }

    // 判断要拷贝的文件类型: 普通文件
    if(S_ISREG(info.st_mode)){
        FILE *fp1 = fopen(argv[1], "r");
        FILE *fp2 = fopen(argv[2], "w");
        if(fp1 == NULL || fp2 == NULL){
            perror("open file failed");
            if(fp1) fclose(fp1);
            if(fp2) fclose(fp2);
            return 1;
        }
        copyFile(fp1, fp2);
        fclose(fp1);
        fclose(fp2);
    }

    //判断是否为目录
    else if(S_ISDIR(info.st_mode)){
        copyDir(argv[1], argv[2]);
    }
    else
        perror("文件类型错误");

}