// C语言编程。如右图的Y字型铁路，左边停了N个车厢，N个车厢分别依次编号为1~N。
// 现希望通过中间的Y字型铁轨，把所有车厢拉到右边，所有车厢不允许走重复的路径。

#include <stdio.h>

#define MAX 20

int n;

void generate(int push, int top, int index, int stack[], int output[])
{
    if (index == n)
    {
        for (int i = 0; i < n; i++)
            printf("%d ", output[i]);
        printf("\n");
        return;
    }

    // 选择1：入栈
    if (push < n)
    {
        int saved = stack[top];
        stack[top] = push + 1;
        generate(push + 1, top + 1, index, stack, output);
        stack[top] = saved;
    }

    // 选择2：出栈
    if (top > 0)
    {
        int val = stack[top - 1];
        int saved_output = output[index]; 
        
        output[index] = val;
        generate(push, top - 1, index + 1, stack, output);
        
        output[index] = saved_output; 
    }
}

int main()
{
    printf("请输入N：");
    scanf("%d", &n);
    
    int stack[MAX] = {0};
    int output[MAX] = {0};
    
    printf("所有可能的出栈序列：\n");
    generate(0, 0, 0, stack, output);
    
    return 0;
}