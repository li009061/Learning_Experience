#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// 数据规模：100万
#define SCALE 1000*1000

// 查找次数统计
static int count;

int sequentialSearch(unsigned data[], int len, int n)
{
    for(int i=0; i<SCALE; i++)
    {
        if(data[i] == n)
            return i;

        count++;
    }

    return -1;
}


void store(unsigned *data, const char *file)
{
    FILE *fp = fopen(file, "w");
    for(int i=0; i<SCALE; i++)
        fprintf(fp, "%06d.%u\n", i, data[i]); // 数据规模：100万（6位数）

    fclose(fp);
}

int main(int argc, char const *argv[])
{
    // 产生一系列无序数据
    srand(time(NULL));
    unsigned *data = calloc(SCALE, sizeof(unsigned));
    for(int i=0; i<SCALE; i++)
        data[i] = rand()%((int)pow(10, rand()%8+5));

    store(data, "random.txt");

    // 不进行任何数据整理，直接进行顺序查找
    unsigned n;
    printf("请输入你要找的正整数：\n");
    while(1)
    {
        scanf("%u", &n);
        int pos = sequentialSearch(data, SCALE, n);

        if(pos == -1)
            printf("找不到你要的数据");
        else
            printf("你要找的数据第%d行", pos);

        printf("【找了%d次】\n", count);
        count = 0;
    }

    return 0;
}
