#include "b.h"
#include "a.h"
#include <strings.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int copyDir(const char *dirSrc, const char *dirDst){
    int count = 0;
    // 获取三个的重要的绝对路径
    char ori_path[1024] = {0};
    char src_path[1024] = {0};
    char dst_path[1024] = {0};

    getcwd(ori_path, 1024);

    // 获取源文件的绝对路径
    if(chdir(dirSrc) == -1){
        fprintf(stderr, "Error:%s\n", strerror(errno));
        return -1;
    }else
        getcwd(src_path, 1024);
   
    // 获取目标文件的绝对路径
    chdir(ori_path);
    if(chdir(dirDst) == -1){
        if(mkdir(dirDst, 0777) == -1){
            perror("mkdir faile");
            return -1;
        }
        chdir(dirDst);
    }
    getcwd(dst_path, 1024);

    printf("ori_path= %s\n", ori_path);
    printf("src_path= %s\n", src_path);
    printf("dst_path= %s\n", dst_path);


    // 循环地将src目录下的各个文件，拷贝到dst中去
    DIR *dp = opendir(src_path);
    if(dp == NULL){
        perror("open src faile");
        return -1;
    }
    while (1)
    {
        //获得read指针
        struct dirent *ep = readdir(dp);
        if(ep == NULL)
            break;
        
        // 忽略.和..
        if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
            continue;
        
        // 进入源目录，获得源目录下的文件的属性
        chdir(src_path);
        struct stat finfo;
        bzero(&finfo, sizeof(finfo));
        if(stat(ep->d_name, &finfo) == -1){
            perror("stat failed");
            continue;
        }

        // 判断要拷贝的文件类型: 普通文件
        if(S_ISREG(finfo.st_mode)){
            FILE *fp1 = fopen(ep->d_name, "r");
            if(fp1 == NULL){
                perror("open src file failed");
                continue;
            }

            // 进入目标目录，创建目标目录下的文件
            chdir(dst_path);
            FILE *fp2 = fopen(ep->d_name, "w");
            if(fp2 == NULL){
                perror("open dst file failed");
                fclose(fp1);
                continue;
            }

            copyFile(fp1, fp2);
            count++;

            fclose(fp1);
            fclose(fp2);
        }       
        
        // 判断要拷贝的文件类型: 目录文件
        else if(S_ISDIR(finfo.st_mode)){

            char subdst[1024] = {0};
            chdir(dst_path);
            mkdir(ep->d_name, 0777);
            chdir(ep->d_name);
            getcwd(subdst, 1024);

            chdir(src_path);
            count += copyDir(ep->d_name, subdst);
        }
    }

    closedir(dp);
    return count;
}
