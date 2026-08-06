// 使用相关系统函数，产生系列随机数，然后采用希尔排序算法，将他们排好序之后输出到屏幕上，
// 并计算排序过程中比较的次数和交换数据的次数。


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comp_count = 0;   // 比较次数   
int move_count = 0;   // 移动次数

void show(int *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d\t", data[i]);
    }
    printf("\n");
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

            move_count++;
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


// void insert_sort(int *data, int len, int n){
//     int i = n;
//     while(i < len){
//         comp_count++;
//         int tmp = data[i];
//         int k;
//         for(k = i-n; k >= 0 && data[k] > tmp; k -= n){
//                 data[k+n] = data[k];
//                 move_count++;
//             }
//             data[k + n] = tmp;
//             i++;
//     }
// }


// void shell_sort(int data[], int len, int n) {
//     if(n < 2){
//         insert_sort(data, len, 1);
//         return;
//     }else{
//         insert_sort(data, len, n);
//         shell_sort(data, len, n-2 );
//     }
// }


int main(void) {
    srand(time(NULL));
    
    int data[100];
    for (int i = 0; i < 100; i++) {
        data[i] = rand() % 10000;
    }
    
    printf("随机数组(未排序):\n");
    show(data, 100);
    
    shell_sort(data, 100);
    
    printf("随机数组(排序后):\n");
    show(data, 100);
    
    printf("比较次数: %d\n", comp_count);
    printf("移动次数: %d\n", move_count);
    
    return 0;
}