// （内核链表）
// 【1】用内核链表存储若干自然数，并将其奇偶重排。
// 比如：
// 链表中存储： 1 2 3 4 5 6 7 8 9 …
// 将其重排成： 1 3 5 7 9 … 8 6 4 2 （奇数升序偶数降序）
// 从后往前遍历,是奇数不变,是偶数尾插法插到最后

#include "../../../3.Linux_kerne_ linked_list/Linux_list/kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int data;

    struct list_head list;
}listnode, *linklist;

void showlist(linklist head){
    //从头往后遍历节点
    struct list_head *pos;
    list_for_each(pos, &head->list){
        struct node *p;
        p = list_entry(pos, struct node,  list);
        printf("%d\t", p->data);
    }
    printf("\n");
}

//删除指定数据节点
void del_list(int i, linklist head){
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &head->list){
        linklist p = list_entry(pos, struct node, list);
        if (p->data == i)
        {
            list_del_init(pos);
            free(p);
            return;
        }
    }
}

linklist initList(){
    linklist head = malloc(sizeof(listnode));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

//添加新节点
linklist newNode(int i){
    struct node *p = malloc(sizeof(listnode));
    if (p != NULL)
    {
        INIT_LIST_HEAD(&p->list);
        p->data = i;
    }
    return p;
}

// 从后往前遍历,是奇数不变,是偶数尾插法插到最后
void rearrange(linklist head){
    if (list_empty(&head->list))
        return;

    struct list_head *pos = head->list.prev;   // 从最后一个节点开始
    while (pos != &head->list){
        struct list_head *prev = pos->prev;   // 保存前一个节点（因为从后往前）
        linklist p = list_entry(pos, struct node, list);
        if (p->data % 2 == 0){
            list_del_init(pos);               // 从原位置删除
            list_add_tail(pos, &head->list);  // 添加到尾部
        }
        pos = prev;   // 继续处理前一个节点
    }
}

//销毁链表
void destroyList(linklist head){
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &head->list){
        linklist p = list_entry(pos, struct node, list);
        list_del_init(pos);
        free(p);
    }
    free(head);
}

int main(int argc, char const *argv[])
{
    //初始化一个带头节点的空链表
    linklist head = initList();
    if (head != NULL)
    {
        printf("链表初始化成功\n");
    }else{
        perror("链表初始化失败");
        exit(0);
    }
    
    //添加新节点
    for (int i = 1; i < 11; i++)
    {
        linklist new = newNode(i);
        list_add_tail(&new->list , &head->list);
    }
    
    printf("初始链表：\n");
    showlist(head);

    //奇数升序偶数降序
    printf("奇数升序偶数降序链表：\n");
    rearrange(head);

    showlist(head);
    //销毁链表
    destroyList(head);
    printf("链表已销毁\n");

    return 0;
}