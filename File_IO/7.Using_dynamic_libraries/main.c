#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
    // 根据配置文件，加载插件
    FILE *fp = fopen("config", "r");
    if(fp == NULL)
    {
        printf("open config error\n");
        return -1;
    }

    char libstr[50] = {0};
    fgets(libstr, 50, fp);
    // libstr[strcspn(libstr, "\n")] = 0;

    fclose(fp);

    // 加载动态库
    void *hanle = dlopen(libstr, RTLD_NOW);
    if(hanle == NULL)
    {
        printf("load lib error: %s\n", dlerror());
        return -1;
    }

    void (*detect)(void);
    detect = dlsym(hanle, "detection");
    if(detect == NULL)
    {
        printf("get func error\n");
        return -1;
    }

	// 潇洒地调用该接口
	detect();
}