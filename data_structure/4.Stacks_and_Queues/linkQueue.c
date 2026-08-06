#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//链表队列的结构体
typedef struct node{
    int data;
    struct node *next;
}node;

//链表队列的管理结构体
typedef struct linkqueue{
    node *fpont;  //队头指针
    node *rear;   //队尾指针
    int size;
}linkqueue;

//初始化链表队列init_linkqueue
linkqueue * init_linkqueue(){
    linkqueue * queue = calloc(1, sizeof(linkqueue));
    if (queue == NULL)
        return NULL;
    
    queue->fpont = queue->rear = NULL;
    queue->size = 0;

    return queue;
}

//添加一个新节点new_node
node * new_node(int data){
    node * new = calloc(1, sizeof(node));
    if (new == NULL)
        return NULL;
    
    new->next = NULL;
    new->data = data;

    return new;
}

//判断队列是否为空is_Empty
bool is_Empty(linkqueue *queue){
    return (queue->size == 0);
}

//将新节点放入队列中(入队)in_linkqueue
void in_linkqueue(linkqueue *queue, node *new){
    if (is_Empty(queue)){
        queue->fpont = queue->rear = new;
    }else{
        queue->rear->next = new;
        queue->rear = new;
    }
    queue->size++;
}

//展示队列中的数据show_linkqueue
void show_linkqueue(linkqueue *queue){
    if (is_Empty(queue)){
        printf("队列为空\n");
        return;
        }
    node *p;
    for(p = queue->fpont; p != NULL; p = p->next){
        printf("%d\t",  p->data);
    }
    printf("\n");
}

//出队out_linkqueue
node * out_linkqueue(linkqueue *queue){
    if (is_Empty(queue))
        return NULL;

    node *p = queue->fpont;
    queue->fpont = p->next;
    p->next = NULL;
    queue->size--;

    return p;
}

int main(int argc, char const *argv[])
{
    //初始化一个空的链表队列
    linkqueue * queue = init_linkqueue();
    if (queue == NULL)
        perror("链表队列初始化失败");
    else
        printf("链表队列初始化成功:\n");

    //入队一些数据
    for (int i = 0; i < 5; i++)
    {
        in_linkqueue(queue, new_node(i));
    }
    
    //展示队列中的数据
    show_linkqueue(queue);

    //展示出队元素
    node *p;
    while (!is_Empty(queue))
    {
        p = out_linkqueue(queue);
        printf("队首元素:%d\t", p->data);
    }
    
    printf("\n");
    return 0;
}
