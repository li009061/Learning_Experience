// 使用相关系统函数，产生系列随机数，然后采用快速排序算法，
// 将他们排好序之后输出到屏幕上，并计算排序过程中比较的次数和交换数据的次数。


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int swap_count = 0;
int comp_count = 0;

void show(int *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d\t", data[i]);
    }
    printf("\n");
}

// // 快速排序的 partition（双指针法）
// int partition(int data[], int low, int high) {
//     int pivot = data[low];   // 选第一个元素为基准
//     int i = low, j = high;
    
//     while (i < j) {
//         // 从右向左找小于 pivot 的
//         while (i < j) {
//             comp_count++;
//             if (data[j] < pivot) 
//             break;
//             j--;
//         }
//         if (i < j) {
//             data[i++] = data[j];
//             swap_count++;
//         }
        
//         // 从左向右找大于 pivot 的
//         while (i < j) {
//             comp_count++;
//             if (data[i] > pivot) 
//             break;
//             i++;
//         }
//         if (i < j) {
//             data[j--] = data[i];
//             swap_count++;
//         }
//     }
    
//     data[i] = pivot;   // 基准归位
//     return i;
// }

void swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
    move_count++;
}

//单指针法
int partition(int data[], int low, int high) {
    int pos = data[high];
    int i = low - 1, j = low;
    while (j < high)
    {
        comp_count++;
        if (data[j] < pos){
            i++;
            move_count++;
            swap(&data[j], &data[i]);
        }

        j++;
    }
    move_count++;
    swap(&data[i+1], &data[high]);
    return i + 1;
}

//快速排序
void quickSort(int data[], int low, int high) {
    if(low < high){
        int pos = partition(data, low, high);
        //左边
        quickSort(data, pos+1, high);
        //右边
        quickSort(data, low, pos-1);
    }
}

int main(void) {
    srand(time(NULL));
    
    int data[100];
    for (int i = 0; i < 100; i++) {
        data[i] = rand() % 10000;
    }
    
    printf("随机数组(未排序):\n");
    show(data, 100);
    
    quickSort(data, 0, 99);
    
    printf("随机数组(排序后):\n");
    show(data, 100);
    
    printf("比较次数: %d\n", comp_count);
    printf("移动次数: %d\n", move_count);
    
    return 0;
}