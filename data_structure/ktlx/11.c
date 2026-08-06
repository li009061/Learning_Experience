// 使用相关系统函数，产生系列随机数，然后采用插入排序算法，将他们排好序之后输出到屏幕上，
// 并计算排序过程中比较的次数和交换数据的次数。

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int move_count = 0;   // 移动次数
int comp_count = 0;   // 比较次数

void show(int *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d\t", data[i]);
    }
    printf("\n");
}

void insersort(int *data, int len) {
    for (int i = 1; i < len; i++) {
        int tmp = data[i];
        int j;
        
        for (j = i - 1; j >= 0; j--) {
            comp_count++;
            if (data[j] <= tmp)   
                break;
            
            data[j + 1] = data[j];
            move_count++;
        }
        
        // 只有发生移动时才放入 tmp
        if (j != i - 1) {         // j 变了，说明有移动
            data[j + 1] = tmp;
            move_count++;         // 最后一次放入
        }
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
    
    insersort(data, 100);
    
    printf("随机数组(排序后):\n");
    show(data, 100);
    
    printf("比较次数: %d\n", comp_count);
    printf("移动次数: %d\n", move_count);
    
    return 0;
}