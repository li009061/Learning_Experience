#ifndef DLINK_LIST_H
#define DLINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    //存放整形数据
    int data;
    //指向相邻节点的指针
    struct node * prev;
    struct node * next;
}node;

//初始化链表
struct node * init_list();
//判断链表是否为空
bool isEmpty(node *head);
//创造一个新的节点
struct node * newNode(int data);
//插入节点（头插法）
void inserhead(node *head, node *new);
//插入节点（尾插法）
void insertail(node *head, node *new);
//展示链表
void show_list(node *head);
//寻找节点
node * find_list(node *head, int data1);
//消除节点
node * remove_list(node *head);
//向前遍历链表
void listForEachPrev(node *head);
//向前遍历链表
void listForEach(node *head);
//销毁链表
void destroy(node *head);

#endif