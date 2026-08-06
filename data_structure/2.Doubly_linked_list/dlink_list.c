#include <stdio.h>
#include "dlink_list.h"

//初始化链表
struct node * init_list(){
    struct node  * head= malloc(sizeof(node));
    if (head != NULL)
    {
        head ->prev = head;
        head ->next = head;
        return head;
    }else{
        return NULL;
    }
}

//判断链表是否为空
bool isEmpty(node *head){
    return head == NULL || head->next == head;
}

//插入节点（头插法）
void inserhead(node *head, node *new){
    new ->prev = head;
    new ->next = head ->next;

    head ->next ->prev = new;
    head ->next = new;
}

//插入节点（尾插法）
void insertail(node *head, node *new){
    new ->next = head;
    new ->prev = head ->prev;

    head ->prev ->next = new;
    head ->prev = new;
}

//创造一个新的节点
struct node * newNode(int data){
    struct node * new = malloc(sizeof(node));
    if (new != NULL)
    {
        new ->data = data;
        new ->prev = new;
        new ->next = new;
    }
    return new;
    
}

//展示链表
void show_list(node *head){
    for (node *tmp = head ->next; tmp != head; tmp = tmp ->next)
    {
        printf("%d\t", tmp ->data);
    }
    printf("\n");
}

//寻找节点
node * find_list(node *head, int data1){
    if (isEmpty(head))
    {
        return NULL;
    }
    node *p;
    for (node *p = head ->next; p != head ; p = p ->next)
    {
        if (p ->data == data1)
        {
            return p;
        }
    }
    return NULL;
}

//消除节点
node * remove_list(node *p){
    p ->next ->prev = p ->prev;
    p ->prev ->next = p ->next;
    p ->next = p;
    p ->prev = p;
    return  p;
}

//向前遍历链表
void listForEachPrev(node *head){
    if (isEmpty(head))
    {
        return;
    }
    
    node *s;
    for ( s = head->prev; s != head; s = s->prev)
    {
        printf("%d\t", s->data);
    }
}

//向前遍历链表
void listForEach(node *head){
    if (isEmpty(head))
    {
        return;
    }
    
    node *s;
    for ( s = head->next; s != head; s = s->next)
    {
        printf("%d\t", s->data);
    }
}

//销毁链表
void destroy(node *head){
    if (isEmpty(head))
    {
        return;
    }
    
    node *p;
    for (p = head->next; p != head; p = head->next)
    {
        remove_list(p);
        free(p);
    }
    free(head);
    
    return;
}