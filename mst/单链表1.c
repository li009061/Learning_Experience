// 3.	已知 p 指向一个单向链表的某个非空节点，pTemp 指向一个独立的新节点。
// 请在不增加变量的情况下，写程序实现把 pTemp 插入到 p 所指向的节点之后。
// （注：每个节点内有一个 next 域指向相邻节点）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    int data;
    struct node *next;
};

int main(void)
{
    struct node *p;
    struct node *pTemp;

    pTemp->next = p->next;
    p->next = pTemp;

    return 0;
}

