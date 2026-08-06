#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//链式栈结构体
struct node
{
    int data;
    struct node *next;
};

//管理链式栈结构体
typedef struct Stacknode
{
    int size;
    struct node *top;
}*LinkStack;

//初始化链式栈
LinkStack init_linkStack(){
    LinkStack stack = calloc(1,sizeof(struct Stacknode));
    if(stack != NULL){
        stack->top = NULL;
        stack->size = 0;
    }
    return stack;
}

//添加新节点
struct node * newNode(int data){
    struct node *s = calloc(1, sizeof(struct node));
    if (s == NULL)
        return NULL;
    else{
        s->data = data;
        s->next = NULL;
    }
    
    return s;
}

//判断栈内是否为空
bool is_Empty(LinkStack stack){
    return (stack->size == 0);
}

//压栈
bool push(LinkStack stack, struct node *new){
    if (new == NULL)
        return false;
    
    new->next = stack->top;
    
    stack->top = new;

    stack->size++;

    return true;
}

//展示栈顶
struct node * show_top(LinkStack stack){
    if (is_Empty(stack))
        return NULL;
    struct node *p = calloc(1, sizeof(struct node));
    if (p == NULL)
        return NULL;
    
    p->data = stack->top->data;
    p->next = NULL;

    return p;
}

//出栈
struct node * pop(LinkStack stack){
    if (is_Empty(stack))
        return false;
    struct node *p = show_top(stack);
    if (p == NULL)
        return NULL;
    
    stack->top = stack->top->next;
    stack->size--;
    p->next = NULL;

    return p;
}

int main(int argc, char const *argv[])
{
    // 初始化链式栈
    LinkStack stack = init_linkStack();
    if (stack == NULL)
    {
        perror("链式栈初始化失败");
    }else
        printf("链式栈初始化成功！\n");

    //添加新的节点，并将新节点压栈
    push(stack, newNode(3));
    push(stack, newNode(4));
    push(stack, newNode(5));

    //展示栈顶
    struct node *p =  show_top(stack);
    printf("%d\n", p->data);
    
    //取栈顶，出栈
    p = pop(stack);
    printf("%d\n", p->data);
    free(p);
    
    printf("%d\n", show_top(stack)->data);
    return 0;
}
