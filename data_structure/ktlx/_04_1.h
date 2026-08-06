#ifndef _04_1_H
#define _04_1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef DATATYPE
#define DATATYPE int
#endif


//定义一个管理结构体
typedef struct list_node{
    DATATYPE data;

    struct list_node *prev;
    struct list_node *next;
}listnode,*linklist;


//初始化链表(init_linklist)
linklist init_linklist(){
    linklist head = malloc(sizeof(listnode));
    if(head == NULL)
        return NULL;
    head->prev = head->next = NULL;
    return head;
}

//判断链表是否为空(is_Empty)
bool is_Empty(linklist head){
    return (head->next == NULL);
}

//增加新节点(newNode)
linklist newNode(DATATYPE data){
    linklist p = malloc(sizeof(listnode));
    if(p == NULL)
        return NULL;
    p->data = data;
    p->prev = p->next = NULL;
    return p;
}

//将新节点插入链表(insertNode)
void inser_linkNode(linklist head, linklist new){
    if(is_Empty(head)){
        head->next = new;
        new->prev = head;
    }else{
        new->next = head->next;
        new->prev = head;
        head->next->prev = new;
        head->next = new;
    }
}

//遍历链表中的数据(list_ForEach)
void list_ForEach(linklist head, void (*handel)(DATATYPE *data)){
    for(linklist p = head->next; p != NULL; p = p->next){
        handel(&p->data);
    }
}


//删除指定数据(delNode)
void delNode(linklist head, bool (*relatively)(DATATYPE data1, DATATYPE data2), DATATYPE data){
    if(is_Empty(head)){
        return;
    }
    for(linklist p = head->next; p != NULL; p = p->next){
        if(relatively(p->data, data)){
            if(p->prev != NULL)
                p->prev->next = p->next;
            if(p->next != NULL)
                p->next->prev = p->prev;
            free(p);
            return;
        }
    }
}

//释放链表free_linklist(linklist head)
void free_linklist(linklist head){
    if(head == NULL)
        return;
    linklist p = head->next;
    while(p != NULL){
        linklist tmp = p;
        p = p->next;
        free(tmp);
    }
    free(head);
}





#endif