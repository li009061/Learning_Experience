// 为上述学生链表，提供指定删除某个学生的功能。

// struct student
// {
//     char *name;
//     int   age;
// };

// int main()
// {
//     struct student Jack = {"Jack", 21};
//     struct student Bill = {"Bill", 23};
//     struct student Rose = {"Rose", 18};
// }

#include "../3.Linux_kerne_ linked_list/Linux_list/kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义一个管理学生的结构体
struct student
{
    char *name;
    int age;
    struct list_head list;
};

//初始化链表init_linklist
struct student * init_linklist(){
    struct student *head = malloc(sizeof(struct student));
    if(head != NULL){
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

//添加新节点newNode
struct student * newNode(char *name, int age){
    struct student *node = malloc(sizeof(struct student));
    if(node != NULL){
        INIT_LIST_HEAD(&node->list);
        node->name = name;
        node->age = age;
    }
    return node;
}

//遍历节点showlist
void showlist(struct student *head){
    //从头往后遍历节点
    struct student *p;
    list_for_each_entry(p, &head->list, list){
        printf("%s %d\t", p->name, p->age);
    }
    printf("\n");
}

//删除指定数据节点del_list
void del_list(struct student *head, char *name){
    struct student *p, *tmp;
    list_for_each_entry_safe(p, tmp, &head->list, list){
        if(strcmp(p->name, name) == 0){
            list_del(&p->list);
            free(p);
            return;
        }
    }
}

//销毁链表destroylist
void destroylist(struct student *head){
    struct student *p, *tmp;
    list_for_each_entry_safe(p, tmp, &head->list, list){
        list_del(&p->list);
        free(p);
    }
    free(head);
}

int main(){
    struct student *head = init_linklist();
    if(head == NULL)
        return 1;

    struct student Jack = {"Jack", 21};
    struct student Bill = {"Bill", 23};
    struct student Rose = {"Rose", 18};

    list_add_tail(&newNode(Jack.name, Jack.age)->list, &head->list);
    list_add_tail(&newNode(Bill.name, Bill.age)->list, &head->list);
    list_add_tail(&newNode(Rose.name, Rose.age)->list, &head->list);

    printf("初始链表:\n");
    showlist(head);

    del_list(head, "Bill");
    printf("删除 Bill 后:\n");
    showlist(head);

    destroylist(head);
    return 0;
}