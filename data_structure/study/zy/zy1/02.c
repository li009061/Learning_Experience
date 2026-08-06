// 罗马人占领乔塔帕特后，犹太人与Josephus及他的朋友躲到一个洞中，族人决定宁愿死也不要被敌人找到，于是决定了一个自杀方式，
// 所有人排成一个圆圈，由第1个人开始报数，每报数到第3人该人就必须自杀，然后再由下一个重新报数，直到所有人都自杀身亡为止。
// 然而Josephus和他的朋友并不想死，Josephus要他的朋友先假装遵从，他将朋友与自己安排在两个特殊的位置，于是逃过了这场死亡游戏。
// 现在假设有n个人形成一个单向循环链表，求最后剩余的两个节点。

#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next; 
}node;


//添加新节点
struct node * new_node(int a){
    struct node *new = malloc(sizeof(struct node));
    if (new != NULL)
    {
        new ->data = a;
        new ->next = new;
    }
    return new;
}


// 尾插法（不带头节点）
void add_tail(node **head, node *new) {
    if (*head == NULL) {
        // 空链表：新节点成为唯一节点，next 指向自己
        new->next = new;
        *head = new;
        return;
    }

    // 找到尾节点（tail->next == head）
    node *tail = *head;
    while (tail->next != *head) {
        tail = tail->next;
    }

    // 插入新节点
    new->next = *head;
    tail->next = new;
}

// 显示链表（遍历一圈回到 head）
void show_list(node *head) {
    if (head == NULL) {
        printf("链表为空\n");
        return;
    }

    node *p = head;
    printf("链表内容: ");
    do {
        printf("%d\t", p->data);
        p = p->next;
    } while (p != head);
    printf("\n");
}

// 释放整个链表
void free_list(node **head) {
    if (*head == NULL) return;

    node *p = *head;
    node *start = *head;
    if (p != NULL) {
        do {
            node *next = p->next;
            free(p);
            p = next;
        } while (p != start);
    }
    *head = NULL;
}

// 返回链表的尾节点（不带头节点）
node *find_tailnode(node *head) {
    if (head == NULL) 
        return NULL;

    node *p = head;
    while (p->next != head) {
        p = p->next;
    }
    return p;
}

// 删除指定节点
void remove_node(node **head, int data){
    if(*head == NULL)
        return;

    
}

int main(int argc, char const *argv[])
{
    int count = 0;
    node *head = NULL;
    // 插入 n 个节点
    int n;
    printf("请输入有多少人参加：\n");
    while(scanf("%d", &n) != 1){
        printf("请输入整数！\n");
        while(getchar() != '\n');
    }
    for (int i = 1; i <= n; i++)
    {
        add_tail(&head, new_node(i));
        count++;
    }

    show_list(head);
    printf("%d\n", count);

    node *p = head;
    node *s = find_tailnode(head);
    // 3.每过三删除节点
    while (count > 2)
    {
        for (int step = 1; step < 3; step++)
        {
            p = p->next;
            s = s->next;
        }
        if (p == head) {
            head = p->next;
        }
        s->next = p->next;
        free(p);
        p = s->next;
        count--;
    }
    
    //4.展示链表
    show_list(head);

    //5.释放链表
    free_list(&head);

    return 0;
}
