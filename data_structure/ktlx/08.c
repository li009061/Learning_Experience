// 使用链式栈，实现十进制转八进制：键盘输入一个十进制数，经过链式栈的相关算法，输出八进制数。


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//定义栈中元素的结构体
typedef struct pnode{
    int data;
    struct pnode *next;

}node;

//定义管理结构体
typedef struct stack{
    int size;
    node *top;

}linkstack;

//初始化一条新链栈(initstack)
linkstack *initstack(){
    linkstack *stack = calloc(1, sizeof(linkstack));
    if(stack == NULL)
        return NULL;
    
    stack->top = NULL;
    stack->size = 0;
 
    return stack;
}

//判断链栈是否为空(is_Empty)
bool is_Empty(linkstack *stack){
    return (stack->size == 0);
}

//入栈(push)
bool push(linkstack *stack, node *new_node){
    if (new_node == NULL)
        return false;
    
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;

    return true;
}

//定义一个新节点(newNode)
node *newNode(int data){
    node * p = calloc(1, sizeof(node));
    if(p == NULL){
        return NULL; 
    }
    p->data = data;
    p->next = NULL;
    return p;
}

//将整数转为八进制(change)
bool change(linkstack *stack, int data){
    if (data == 0) {
        push(stack, newNode(0));
        return true;
    }
    while(data > 0){
        int n = data % 8;
        if(!push(stack, newNode(n)))
            return false;
        data /= 8;
    }
    return true;
}

    

//出栈(pop)
bool pop(linkstack *stack){
    if(is_Empty(stack))
        return false;

    node *tmp = stack->top;
    stack->top = stack->top->next;
    free(tmp);
    stack->size--;

    return true;
}

//展示栈顶元素(show_Top)
void show_Top(linkstack *stack){
    if(is_Empty(stack))
        return ;
    
    printf("%d", stack->top->data);
}

//释放链栈(freestack)
void freestack(linkstack *stack){
    if(is_Empty(stack)){
        free(stack);
        return ;
    }
    
    node *p;
    for(p = stack->top; p != NULL; p = stack->top){
        stack->top = stack->top->next;
        free(p);
    }
    free(stack);
    
    return ;
}


int main(int argc, char const *argv[])
{
    //初始化一条管理结构体
    linkstack *stack = initstack();
    if(stack == NULL){
        printf("链栈初始化失败\n");
        return 1;
    }else{
        printf("链栈初始化成功\n");
    }

    //从键盘中读取整数
    int n;
    while(1){
        if(scanf("%d", &n) == 1){
            if(n < 0){
                printf("不支持负数\n");
                continue;
            }else
                break;
        }else{
            while(getchar() != '\n');
            printf("输入错误，请重新输入:\n");
            continue;
        }
    }

    //将整数转为八进制
    change(stack, n);
    printf("八进制数为:\n");
    while(!is_Empty(stack)){
        show_Top(stack);
        pop(stack);
    }
    printf("\n");
    //释放结构体
    freestack(stack);
    printf("链栈已释放\n");
    return 0;
}

