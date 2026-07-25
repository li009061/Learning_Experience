#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    //1.搞个随机数组
    int data[10];
    for (int i = 0; i < 10; i++)
    {
        data[i] = rand()%1000;
    }
    
    //2.冒泡排序
    for (int i = 0; i < 10; i++)
    {
        int done = 0;
        //两两比较
        for (int j = 0; j < 10 - i -1; j++)
        {
            if(data[j] > data[j+1]){
                int tmp;
                tmp = data[j];
                data[j] = data[j+1];
                data[j+1] = tmp;

                done = 1;
            }
        }
        if (!done){
            break;
        }
    }
    
    //3.输出验证
    printf("排序后：");
    for (int i = 0; i < 10; i++) printf("%d ", data[i]);
    printf("\n");
    return 0;
}
