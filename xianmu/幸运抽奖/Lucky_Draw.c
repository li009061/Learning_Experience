#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_PEOPLE 500
#define MAX_NAME_LEN 50

int main(){
    //幸运抽奖
    printf("幸运抽奖:\n");

    //打开文件并读取数据
    FILE *fp = fopen("英雄名单.txt", "r");
    if(fp == NULL){
        printf("文件打开失败\n");
        return 1;
    }

    //将数据存入数组
    char names[MAX_PEOPLE][MAX_NAME_LEN];
    int count = 0;
    
    while (fgets(names[count], MAX_NAME_LEN, fp)) {
        names[count][strcspn(names[count], "\r\n")] = '\0';
        count++;
        if (count >= MAX_PEOPLE) 
            break;
    }
    //关闭文件
    fclose(fp);

    //产生随机数
    time_t t = time(NULL);
    srand(t);

    int idx = 0;
    int delay = 30000;
    //开始闪屏
    for(int i =0; i < 18; i++){
        
        idx = rand() % count;
        // 输出当前抽奖名字到标准错误：\r回到行首，\033[K清除整行，实现同一位置滚动闪烁效果
        fprintf(stderr, "\r%s\033[K", names[idx]);
        usleep(delay);
        delay = delay *5 /4;
    }

    printf("\n最终中奖者是: %s!\n", names[idx]);
    return 0;
}