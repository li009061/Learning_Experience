// 1、（单向链表）
// 建立一个包含若干整数的单向链表，比如: 
// 1,2,3,4,5
// 通过某些算法将其中各个节点逆转，比如: 
// 5,4,3,2,1
#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

struct node * init_list(void){
    struct node *head = malloc(sizeof(struct node));
    if (head != NULL)
    {
        head ->next = NULL;
    }
    

    return head;
}

void add_node_tail(struct node *head, struct node *new){
    struct node *tail = head;
    while (tail ->next != NULL)
    {
        tail = tail ->next;
    }

    tail->next = new;
    
}

struct node * new_node(int n){
    struct node * new = malloc(sizeof(struct node));
    if (new != NULL)
    {
        new ->data = n;
        new ->next = NULL;
    }
    return new;
    
}

void show_list(struct node *head){
    for (struct node *p = head ->next; p != NULL ; p = p ->next)
    {
        printf("%d\t",p ->data);
    }
    printf("\n");
}

void reverse(struct node *head){

    struct node *p = head ->next;
    head ->next = NULL;

    struct node *n;
    while (p != NULL)
    {
    n = p ->next;
    p ->next = head ->next;
    head ->next = p;
    p = n;
    }
}

int main(int argc, char const *argv[])
{
    struct node *head = init_list();
     for (int i = 0; i <= 10; i++)
     {
        add_node_tail(head,new_node(i));
     }
     
     show_list(head);
     
     reverse(head);
     
     show_list(head);
     return 0;
}
