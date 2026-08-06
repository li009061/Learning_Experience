#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//设计单链表的节点
typedef struct node
{
    int data;
    struct node *next;
}node, *list;

//常用接口声明
node *list_init(void);
node * new_node(int data);

void add_node(node *head, node *new);
void add_node_tail(node *list, node *new);

void list_for_each(node *list);
void list_destroy(node *list);


#endif