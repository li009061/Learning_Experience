//使用顺序栈，完成十进制转十六进制

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//定义一个顺序栈的结构体
typedef struct node
{
    int *data;
    int size;
    int top; 
}node;


//初始化结构体
struct node * init_stack(int room){
    node * stack = calloc(1, sizeof(node));
    if (stack == NULL)
        return NULL;
    
    stack->data = calloc(room, sizeof(int));
    if (stack->data == NULL)
    {
        free(stack);
        return NULL;
    }

    stack->size = room;
    stack->top = -1;

    return stack;
}

//判断栈是否为空
bool is_Empty(node *stack){
    return (stack->top == -1);
}

//判断栈是否满了
bool is_full(node *stack){
    return (stack->top >= stack->size-1);
}

//入栈
bool push(node *stack, int n){
    if (is_full(stack))
        return false;
    
    stack->top++;
    stack->data[stack->top] = n;
    
    return true;
}

//出栈
int pop(node *stack){
    if (is_Empty(stack))
        return -1;

    return stack->data[stack->top--];
}

//展示栈顶
void show_stack(node *stack){
    if(is_Empty(stack))
        return;
    printf("%d",stack->data[stack->top]);
    return;
}

//销毁栈
void destroy_stack(node *stack){
    if (stack == NULL)
        return;
    free(stack->data);
    free(stack);
}

//将十进制转换成十六进制
void Decimal_to_hexadecimal(node *stack,int n){
    if (n == 0)
    {
        push(stack, 0);
        return;
    }
    
    int a = n;
    while (a > 0)
    {
        push(stack, a % 16);
        a = a / 16;
    }
    return;
}

//打印十六进制字符
void print_hex(int val){
    if (val < 10)
        printf("%d", val);
    else
        printf("%c", 'A' + val - 10);
}

int main(int argc, char const *argv[])
{
    //初始化结构体
    struct node *stack = init_stack(100);    

    //输入十进制数
    int n;
    printf("请输入一个十进制数:\n");
    while (1)
    {
        if (scanf("%d", &n) != 1)
        {
            printf("输入错误，请重新输入:\n");
            while(getchar() != '\n');
        }else
            break;
    }
    
    if (n < 0)
    {
        printf("不支持负数\n");
        destroy_stack(stack);
        return 0;
    }
    
    //将十进制转换十六进制
    Decimal_to_hexadecimal(stack, n);

    //展示
    printf("0x");
    while (!is_Empty(stack))
    {
        print_hex(pop(stack));
    }
    
    printf("\n");

    destroy_stack(stack);
    return 0;
}
