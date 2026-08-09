#include <stdio.h>
#include "array_utils.h"

int main() {
    
   // 测试一维数组函数
    int arr[] = {-2, 5, -1, 6, -4, -8, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("原数组：");
    print_array(arr, n); 

    // 测试最大子数组和
    int max_sum = max_subarray_sum(arr, n);
    printf("最大子数组和：%d\n", max_sum);

   // 测试二维数组函数
    int m = 2, c = 3;
    int src[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int dst[2][3];

    copy_array_2d(m, c, (int*)src, (int*)dst);

    printf("复制后的二维数组：\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", dst[i][j]);
        }
        printf("\n");
    }

    return 0;
}