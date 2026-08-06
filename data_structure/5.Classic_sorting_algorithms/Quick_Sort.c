#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int comp_count = 0;
int swap_count = 0;

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

int partition(int data[], int len)
{
    if(len <= 1)
        return 0;

    int i = 0;
    int j = len-1;
    while(i < j)
    {
        // 从右向左比较，顺序j--，逆序交换
        comp_count++;
        while(data[i]<=data[j] && i<j)
            j--;
        swap(&data[i], &data[j]);

        // 从左向右比较，顺序i++，逆序交换
        comp_count++;
        while(data[i]<=data[j] && i<j)
            i++;
        swap(&data[i], &data[j]);
    }

    return i;
}

void quickSort(int data[], int len)
{
    if(len <= 1)
        return;

    int pivot = partition(data, len);

    quickSort(data, pivot);
    quickSort(data+pivot+1, len-pivot-1);
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

    printf("快速排序: ");
    quickSort(data, 100);
    show(data, 100);

    printf("总共比较次数: %d\n", comp_count);
    printf("总共交换次数: %d\n", swap_count);

    return 0;
}