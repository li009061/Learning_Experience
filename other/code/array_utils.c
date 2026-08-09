#include <stdio.h>
#include "array_utils.h"

//求最大子数组和
int max_subarray_sum(int *arr, int n){
    if (n <= 0) 
    return 0; 
    int max=arr[0], sum=0;
    int i;
    for(i = 0; i < n; i++)
    {
        sum += arr[i];

        if(sum > max)
            max = sum;

        else if(sum < 0)
            sum = 0;
    }
    return max;
}

// 复制二维数组
void copy_array_2d(int m, int n, int *src, int *dst) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            dst[i * n + j] = src[i * n + j];
        }
    }
}

// 打印一维数组
void print_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}