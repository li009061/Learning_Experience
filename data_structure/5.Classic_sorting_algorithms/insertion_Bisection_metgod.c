#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_LEN 100   //默认数组长度

int exchange = 0;   //交换次数
int commper = 0;    //比较次数

//展示数组内的数据
void show(int data[], int len){
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", data[i]);
    }
    printf("\n");
}

//二分法：在有序数组 data[0..len-1] 中找到 a 的插入位置
int FindPos(int *data, int len, int a){
    int low = 0;
    int high = len - 1;

    while(low <= high){
        int mid = (low + high) / 2;
        commper++;
        if(data[mid] < a)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return low;
}


//插入排序
void insertion_sort(int data[], int len){
    for(int i = 1; i < len; i++){
        //采用二分法找到合适位置
        int pos = FindPos(data, i, data[i]);

        //批量移动数据
        int tmp = data[i];
        for(int k = i - 1; k >= pos; k--){
            data[k+1] = data[k];
            exchange++;
        }
        data[pos] = tmp;
    }
}


int main(int argc, char const *argv[])
{
    //输入数组长度
    int len = DEFAULT_LEN;
    printf("请输入数组长度（直接回车使用默认值 %d）:", DEFAULT_LEN);
    int input_len;
    if(scanf("%d", &input_len) == 1 && input_len > 0){
        len = input_len;
    }
    while(getchar() != '\n');

    //动态分配数组
    int *data = malloc(sizeof(int) * len);
    if(data == NULL){
        printf("内存分配失败\n");
        return 1;
    }

    //生成一些随机数
    srand(time(NULL));
    for (int i = 0; i < len; i++)
    {
        data[i] = rand() % 1000;
    }

    //展示随机数
    printf("未排序的随机数:\n");
    show(data, len);

    //将这些随机数排序
    insertion_sort(data, len);

    //展示排序好的随机数
    printf("排序好之后的随机数:\n");
    show(data, len);

    printf("总共比较次数: %d\n", commper);
    printf("总共移动次数: %d\n", exchange);

    free(data);
    return 0;
}
