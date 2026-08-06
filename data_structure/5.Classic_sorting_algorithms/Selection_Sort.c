#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int comp_count = 0; // 数据比较次数
int swap_count = 0; // 数据交换次数

void show(int data[], int len)
{
    int i;
    for(i=0; i<len; ++i)
    {
        printf("%d\t", data[i]);
    }
    printf("\n");
}

void swap(int *a, int *b)
{
    swap_count++;

    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void selectionSort(int data[], int len)
{
    for(int i=0; i<len; i++)
    {
        int min=i;

        for(int j=i+1; j<len; j++)
        {
            comp_count++;
            if(data[j] < data[min])
                min = j;
        }

        swap(&data[i], &data[min]);
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    int i, data[100];
    for(i=0; i<100; ++i)
    {
        int exp = (int)pow(10, rand()%3+3);
        data[i] = rand() % exp;
    }
    printf("随机序列: ");
    show(data, 100);

    selectionSort(data, 100);
    printf("选择排序: ");
    show(data, 100);

    printf("总共比较次数: %d\n", comp_count);
    printf("总共交换次数: %d\n", swap_count);
    return 0;
}