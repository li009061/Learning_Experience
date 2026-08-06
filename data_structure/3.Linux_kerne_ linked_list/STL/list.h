//lsit.h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef DATATYPE
#define DATATYPE int
#endif

typedef DATATYPE datatype;

typedef struct node{
    datatype data;

    struct node *prev;
    struct node *next;
}listnode, *linklist;

static listnode * initList()
{
    listnode * head = (listnode *)malloc(sizeof(listnode));

    if(head != 0)
    {
        head->prev = head;
        head->next = head;
    }

    return head;
}

//创造一个新的节点
static listnode *newNode(datatype data){
    struct node * new = malloc(sizeof(listnode));
    if (new != NULL)
    {
        new ->data = data;
        new ->prev = new;
        new ->next = new;
    }
    return new;
    
}

// 判断链表是否为空，与用户实际数据无关
static bool isEmpty(listnode *head)
{
    return head->next == head;
}

//插入节点（尾插法）
static void insertail(listnode *head, listnode *new){
    new ->next = head;
    new ->prev = head ->prev;

    head ->prev ->next = new;
    head ->prev = new;
}

//遍历链表
static void listForEach(linklist head, void (*handler)(datatype *data)){
    for (listnode *p = head->next; p != head; p= p->next)
    {
        handler(&p->data);
    }
}

//寻找节点
static linklist findNode(linklist head, datatype data, 
                bool (*equal)(datatype daat1, datatype data2)){
                    for (linklist p = head->next; p != head; p=p->next)
                    {
                        if (equal(p->data, data))
                        {
                            return p;
                        }
                    }
                    return NULL;                   
}

static bool remove_list(linklist p){
    if (p == NULL || isEmpty(p))
    {
        return false;
    }
    
    p ->next ->prev = p ->prev;
    p ->prev ->next = p ->next;
    p ->next = p;
    p ->prev = p;
    return  p;
}