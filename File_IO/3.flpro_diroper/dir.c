#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]) // ./a.out xxx/
{
    if(argc != 2)
    {
        printf("Usage: %s <dir>\n", argv[0]);
        exit(1);
    }

    // 判定参数的合法性
    struct stat finfo;
    bzero(&finfo, sizeof(finfo));
    stat(argv[1], &finfo);

    if(!S_ISDIR(finfo.st_mode))
    {
        printf("请指定一个目录\n");
        exit(1);
    }

    // 将目录中的文件列出来
    //opendir()只是获得一个目录的句柄，而不是目录中的文件
    
    char path[100];
    bzero(path, 100);
    printf("当前所在路径:%s\n",getcwd(NULL,100));
    DIR *dp = opendir(argv[1]);
    
    
    chdir(argv[1]);
    printf("当前所在路径:%s\n",getcwd(NULL,100));

    //进入指定的目录
    chdir(argv[1]);

    while(1)
    {
        struct dirent * ep = readdir(dp); // ep --> 目录项（文件）
        if(ep == NULL)
        {
            break;
        }

        // 略过所有隐藏文件
        if(ep->d_name[0] == '.')
            continue;

        bzero(&finfo,sizeof(finfo));
        stat(ep->d_name, &finfo);
        printf("%ld ", finfo.st_size);

        printf("%s\n", ep->d_name);
    }

    // 释放资源
    closedir(dp);

    return 0;
}
