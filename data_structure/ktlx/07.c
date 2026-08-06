// 使用顺序栈，接收键盘的输入，实现如下功能：
// 输入数字时，依次入栈。
// 输入字母时，依次出栈。
// 每次入栈或者出栈，都将顺序栈中的各个元素输出出来。

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//栈的管理结构体
typedef struct node
{
    int *data;
    int size;
    int top;
}orderstack;

//初始化栈init_stack()
orderstack *init_stack(int cap){
    orderstack *stack = calloc(1,sizeof(orderstack));
    if (stack == NULL)
        return NULL;

    stack->data = calloc(cap, sizeof(int));
    if (stack->data == NULL){
        free(stack);
        return NULL;
    }
    
    stack->size = cap;
    stack->top = -1;

    return stack;
}

//是否为空栈
bool is_Empty(orderstack *stack){
    return (stack->top == -1);
}

//是否为满栈
bool is_Full(orderstack *stack){
    return (stack->top >= stack->size-1);
}

//入栈push
bool push(orderstack *stack, int data){
    if(is_Full(stack))
        return false;
    
    stack->data[++stack->top] = data;
    return true;
}

//出栈pop
bool pop(orderstack *stack){
    if(is_Empty(stack))
        return false;

    stack->top--;
    return true;
}

//展示栈内数据
void show(orderstack *stack){
    if (is_Empty(stack))
        return;

    int i;
    for (i = stack->top; i >= 0; i--)
    {
        printf("%d\t", stack->data[i]);
    }
    printf("\n");
    return;
}



int main(int argc, char const *argv[])
{
    //初始化栈
    orderstack *stack = init_stack(10);
    if(stack == NULL){
        perror("栈初始化失败");
        return 1;
    }
    printf("顺序栈初始化成功！\n");

    //从键盘中接收输入，分别出栈，入栈
    char ch;
    printf("请输入数字（入栈）或字母（出栈），其他字符退出：\n");
    while (scanf("%c", &ch) == 1)
    {
        if (ch == '\n' || ch == ' ' || ch == '\t') {
            continue;
        }

        if (ch >= '0' && ch <= '9') {
            // 输入数字，入栈
            if(!push(stack, ch - '0')){
                printf("入栈失败，空间不足！\n");
            }
            show(stack);
        } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            // 输入字母，出栈
            if (!pop(stack)){
                printf("出栈失败，栈为空！\n");
            } else {
                show(stack);
            }
        } else {
            // 其他字符，退出
            break;
        }
    }

    free(stack->data);
    free(stack);
    return 0;
}
