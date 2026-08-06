#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

void bubbleSort(int data[], int len)
{
    int k=0;
    while(1)
    {
        bool done = true;

        int i;
        for(i=0; i<len-1-k; i++)
        {
            comp_count++;

            if(data[i] <= data[i+1])
            {
                continue;
            }
        
            swap(&data[i], &data[i+1]);
            done = false;
        }

        if(done)
            break;
        k++;
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    int i, data[100];
    for(i=0; i<100; ++i)
    {
        data[i] = rand() % 1000;
    }
    printf("随机序列: ");
    show(data, 100);

    bubbleSort(data, 100);  // 按升序排列
    printf("冒泡排序: ");
    show(data, 100);

    printf("总共比较次数: %d\n", comp_count);
    printf("总共交换次数: %d\n", swap_count);
    return 0;
}