#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int exchange = 0;   //交换次数
int commper = 0;    //比较次数

//展示数组内的数据
void show(int data[]){
    for (int i = 0; i < 100; i++)
    {
        printf("%d\t", data[i]);
    }
    printf("\n");
}

// 插入排序
void inssertion_sort(int data[]){
    int i , j;
    for(i = 0; i < 100; i++){
        int tmp = data[i];
        for (j = i - 1; j >= 0; j--)
        {
            commper++;
            if(data[j] < tmp){
                break;
            }else{
                exchange++;
                data[j+1] = data[j];
            }
        }
        exchange++;
        data[j+1] = tmp;
    }
}

int main(int argc, char const *argv[])
{
    //生成一些随机数
    srand(time(NULL));
    int i, data[100];
    for ( i = 0; i < 100; i++)
    {
        data[i] = rand() % 10000;
    }
    
    //展示随机数
    printf("未排序的随机数:\n");
    show(data);

    //将这些随机数排序
    inssertion_sort(data);

    //展示排序好的随机数
    printf("排序好之后的随机数:\n");
    show(data);

    printf("总共比较次数: %d\n", commper);
    printf("总共移动次数: %d\n", exchange);

    return 0;
}
