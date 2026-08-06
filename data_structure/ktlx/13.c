// 使用相关系统函数，产生系列随机数，然后采用 冒泡排序 算法，
// 将他们排好序之后输出到屏幕上，并计算排序过程中比较的次数和交换数据的次数。

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


//比较和交换次数
int swap_count = 0;
int comp_count = 0;

//展示随机数
void show(int *data, int len){
    for(int i = 0; i < len; i++){
        printf("%d\t", data[i]);
    }
    printf("\n");
}

//交换
void swap(int *data, int *data1){

    int tmp;
    tmp = *data;
    *data = *data1;
    *data1 = tmp;
}


//冒泡排序
void Bubblesort(int *data, int len){
    int i;
    for(i = 0; i < len-1; i++){
        int j;
        bool is_swap = false;
        for( j = 0; j < len - i -1; j++){
            comp_count++;
            if(data[j] > data[j+1]){
                swap(&data[j], &data[j+1]);
                swap_count++;
                is_swap = true;
            }
        }
        if(!is_swap)
            break;
    }
}



int main(int argc, char const *argv[])
{
    //产生随机数
    srand(time(NULL));

    //将产生的随机数放入数组
    int i, data[100];
    for (i = 0; i < 100; i++)
    {
        data[i] = rand()%10000;
    }
    
    //展示未排序的随机数
    printf("随机数组(未排序):\n");
    show(data, 100);

    //排序随机数
    Bubblesort(data, 100);
    printf("随机数组(排序):\n");
    show(data, 100);

    //展示排序好的随机数
    printf("总共比较次数: %d\n", comp_count);
    printf("总共移动次数: %d\n", swap_count);
    return 0;
}
