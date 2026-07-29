#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct student
{
    char name[32];
    float yuwen;
    float shuxue;
    float yingyu;
};

float sum_up_yuwen(struct student cls[], int count)
{
    float sum = 0;
    for(int i=0; i<count; i++)
        sum += cls[i].yuwen;
    return sum;
}

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("score.txt", "r");
    if(fp == NULL)
    {
        perror("打开文件失败");
        return 0;
    }

    struct student GZ2622[10];
    int i, ret;
    for(i=0;; i++)
    {
        ret = fscanf(fp, "%s%f%f%f", GZ2622[i].name, &GZ2622[i].yuwen,
                              &GZ2622[i].shuxue, &GZ2622[i].yingyu);
        if(ret == EOF)
            break;
    }
    printf("语文总分: %f\n", sum_up_yuwen(GZ2622, i));

    return 0;
}
