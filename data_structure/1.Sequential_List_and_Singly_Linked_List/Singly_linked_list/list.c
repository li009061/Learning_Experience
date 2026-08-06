#include "list.h"

node *list_init(void){
    node *head = malloc(sizeof(node));
    if (head != NULL)
    {
        head ->next = NULL;
    }
    return head;
}
node * new_node(int data){
    node *new = malloc(sizeof(node));
    if (new != NULL)
    {
        new ->data = data;
        new ->next = NULL;
    }
    return new;
}

void add_node(node *head, node *new){
    if (head == NULL || new == NULL)
    {
        return;
    }
    
    new ->next = head->next;
    head ->next = new;
}

void add_node_tail(node *list, node *new){
    if (list == NULL || new == NULL) return;
    //0.找到尾点指针
    node *tail;
    for (tail = list; tail ->next != NULL; tail = tail ->next);
        //1.将尾指针指向new
        tail ->next = new;
}

void list_for_each(node *list){
    for (node *p = list ->next; p != NULL; p = p->next)
    {
        printf("%d\t", p ->data);
    }
    printf("\n");
}

void list_destroy(node *list){
    if (list == NULL) return;
    
    // node *p = list;
    // while (p != NULL) {
    //     node *tmp = p;
    //     p = p->next;
    //     free(tmp);
    // }
    node *n;
    for (node *p = list; p != NULL; p = n)
    {
        n= p ->next;
        free(p);
    }
}
