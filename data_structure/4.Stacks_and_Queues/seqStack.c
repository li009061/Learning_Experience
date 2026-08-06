#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//管理顺序栈的结构体
struct seqStack
{
    int *data;  //入口
    int room;   //容量
    int top;    //栈顶
};


//初始化顺序栈的函数(init_sqestack)
struct seqStack * init_seqStack(int room){
    struct seqStack *Stack = malloc(sizeof(struct seqStack));
    if (Stack != NULL)
    {
        Stack->data = calloc(room,sizeof(int));
        if (Stack->data == NULL)
        {
            free(Stack);
            return NULL;
        }
        Stack->room = room;
        Stack->top = -1;
    }
    return Stack;
}

//判断栈是否满了
bool is_full(struct seqStack *stack){
    return stack->top >= stack->room-1;
}

//判断栈是否为空
bool is_Empty(struct seqStack *stack){
    return stack->top == -1;
}

//将数据入栈(push)
bool push(struct seqStack *stack, int data){
    if (is_full(stack))
    {
        return false;
    }
    stack->data[++stack->top] = data;
    return true;
}

//展示栈顶数据
bool show_top(struct seqStack *stack,int *data){
    if (is_Empty(stack))
        return false;

    *data = stack->data[stack->top];
    return true;
}

//将栈顶的数据出栈
bool show_pop_stack(struct seqStack *stack,int *data){
    if (!show_top(stack, data))
        return false;
        
    stack->top--;
    return true;
}

int main(int argc, char const *argv[])
{
    //分配空间初始化顺序栈
    struct seqStack *stack = init_seqStack(10);
    if (stack == NULL)
    {
        perror("顺序栈初始化失败");
        exit(0);
    }else
        printf("顺序栈初始化成功！\n");

    //存放一些数据入栈
    push(stack, 3);
    push(stack, 4);
    //展示栈顶
    int m;
    show_top(stack, &m);
    push(stack, 5);

    //将栈中的数据取出来，展示
    while (!is_Empty(stack))
    {
        show_pop_stack(stack, &m);
        printf("%d\t", m);
    }
    printf("\n");

    return 0;
}
