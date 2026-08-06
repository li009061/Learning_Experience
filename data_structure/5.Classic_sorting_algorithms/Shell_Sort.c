#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>



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
    return;
}

//                    起点    节点个数    间距
void insert_sort(int data[], int len, int delta)
{
    if(len <= 1)
        return;

    for(int i=delta; i<len*delta; i+=delta)
    {
        int j, tmp = data[i];
        for(j=i-delta; j>=0; j-=delta)
        {
            comp_count++;
            if(data[j] < tmp)
                break;

            swap_count++;
            data[j+delta] = data[j];
        }

        data[j+delta] = tmp;
    }
}

void shell_sort(int data[], int len)
{
    if(len <= 1)
        return;

    for(int delta=len/2; delta>0; delta/=2)
    {
        for(int i=0; i<delta; ++i)
        {
            //           起点     节点个数    间距
            insert_sort(data+i, len/delta, delta);
        }
    }
}

int main(void)
{
    // 准备产生一些随机数
    srand(time(NULL));

    int i, data[100];
    for(i=0; i<100; i++)
    {
        int exp = (int)pow(10, rand()%3+3);
        data[i] = rand()%exp;
    }
    printf("随机序列: ");
    show(data, 100);

    printf("希尔排序: ");
    shell_sort(data, 100);
    show(data, 100);

    printf("总共比较次数: %d\n", comp_count);
    printf("总共移动次数: %d\n", swap_count);

    return 0;
}