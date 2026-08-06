#include "kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
        perro("链表初始化失败");
        exit(0);
    }
    
    //添加新节点
    for (int i = 0; i < 5; i++)
    {
        linklist new = newNode(i);
        list_add_tail(&new->list , &head->list);
    }
    
    printf("初始链表：");
    showlist(head);
    
    //删除指定节点
    int n;
    while (1)
    {
        printf("请输入要删除的数据(输入-1退出):");
        if (scanf("%d", &n) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        if (n == -1)
        {
            break;
        }

        del_list(n, head);
        printf("删除后链表：");
        showlist(head);
    }
    
    //销毁链表
    destroyList(head);
    printf("链表已销毁\n");

    return 0;
}