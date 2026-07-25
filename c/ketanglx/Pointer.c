//用指针的方式，计算一个int数组的所有元素的和
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a[5] = { 1 , 5 , 6 , 28 , 74};
    int *p = a;
    int sum;
    for (int i = 0; i < 5; i++)
    {
        sum += *(p + i);
    }
    printf("%d\n", sum);
    return 0;
}


//array == ptr 这句是否正确？
//不正确，1.一个是数组，是用来存放数据的，一个是指针用来放地址的。
//2.数组不可以被赋值，而指针可以改变指向
