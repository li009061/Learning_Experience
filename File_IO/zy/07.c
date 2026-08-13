// (目录操作、文件IO函数)
// 编写一个程序，令其功能尽量向命令 “cp” 靠近。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

void copy_file(FILE *fp1, FILE *fp2)
{
    if(fp1 == NULL || fp2 == NULL)
        return;

    char buf[1024];
    while(1)
    {
        // n就是读取的字节数（因为一块就是一个字节）
        int n = fread(buf, 1, 1024, fp1);
        if(n == 0)
            break;

        fwrite(buf, 1, n, fp2);
    }
}

void copy_dir(const char *src, const char *dst)
{
    // 获取三个的重要的绝对路径
    char ori_path[1024] = {0};
    char src_path[1024] = {0};
    char dst_path[1024] = {0};

    getcwd(ori_path, 1024);

    // 获取源文件的绝对路径
    if(chdir(src) == -1)
    {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return;
    }
    getcwd(src_path, 1024);

    // 获取目标文件的绝对路径
    chdir(ori_path);
    if(chdir(dst) == -1)
    {
        chdir(ori_path);
        if(mkdir(dst, 0777) == -1)
        {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            return;
        }
        chdir(dst);
    }
    getcwd(dst_path, 1024);

    printf("ori_path = %s\n", ori_path);
    printf("src_path = %s\n", src_path);
    printf("dst_path = %s\n", dst_path);

    // 循环地将src目录下的各个文件，拷贝到dst中去
    DIR *dp = opendir(src_path);
    while(1)
    {
        struct dirent *ep = readdir(dp);
        if(ep == NULL)
            break;
        
        // 忽略.和..
        if(strcmp(ep->d_name, ".") == 0 ||
           strcmp(ep->d_name, "..") == 0)
            continue;
        
        // 进入源目录，获得源目录下的文件的属性
        chdir(src_path);
        struct stat finfo;
        bzero(&finfo, sizeof(finfo));
        stat(ep->d_name, &finfo);

        // 判断要拷贝的文件类型: 普通文件
        if(S_ISREG(finfo.st_mode))
        {
            FILE *fp1 = fopen(ep->d_name, "r");

            // 进入目标目录，创建目标目录下的文件
            chdir(dst_path);
            FILE *fp2 = fopen(ep->d_name, "w");

            copy_file(fp1, fp2);

            fclose(fp1);
            fclose(fp2);
        }

        // 判断要拷贝的文件类型: 目录文件
        else if(S_ISDIR(finfo.st_mode))
        {
            char subdst[1024] = {0};
            chdir(dst_path); // 1
            mkdir(ep->d_name, 0777); // 2
            chdir(ep->d_name);
            getcwd(subdst, 1024); // 3

            chdir(src_path);
            copy_dir(ep->d_name,  subdst);
        }
    }
}

int main(int argc, char const *argv[]) // ./mycpy src dst
{
    // 判断参数的合法性
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s src dst\n", argv[0]);
        return 1;
    }

    // 获取源文件的信息
    struct stat finfo;
    bzero(&finfo, sizeof(finfo));
    stat(argv[1], &finfo);

    // 判断要拷贝的文件类型: 普通文件
    if(S_ISREG(finfo.st_mode))
    {
        FILE *fp1 = fopen(argv[1], "r");
        FILE *fp2 = fopen(argv[2], "w");

        copy_file(fp1, fp2);
    }

    // 判断要拷贝的文件类型: 目录文件
    else if(S_ISDIR(finfo.st_mode))
        copy_dir(argv[1], argv[2]);

    else
        fprintf(stderr, "Error: 文件类型不支持\n");

    return 0;
}
