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

struct node * inis_list(void){
    struct node *head = malloc(sizeof(struct node));
    if (head != NULL)
    {
        head ->next = NULL;
    }
    return head;
}

struct node * new_node(int a){
    struct node *new = malloc(sizeof(struct node));
    if (new != NULL)
    {
        new ->data = a;
        new ->next =NULL;
    }
    
}

void add_node_tail(struct node *head, struct node *new){
    if (head == NULL || new == NULL) 
    return;
    struct node *p = head;
    while (p->next != NULL)
    {
        p = p ->next;
    }
    p ->next = new;
}

void show_list(node *head, int count) {
    node *p = head->next;
    for (int i = 0; i < count && p != NULL; i++) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int main() {
    int n = 13;
    node *head = inis_list();
    
    // 1. 建立单向链表（尾指针指向最后一个数据节点）
    node *tail = head;
    for (int i = 1; i <= n; i++) {
        node *new = new_node(i);
        add_node_tail(head, new);
        tail = new; // 记录最后一个节点
    }
    
    // 2. 【关键修正】把尾部接到第一个数据节点，真正成环！
    tail->next = head->next; 
    
    // 3. 准备模拟：从第一个数据节点（编号1）开始报数
    node *prev = tail;      // 前驱指向尾（因为环，尾是头的前驱）
    node *cur = head->next; // 当前指向编号1
    int remaining = n;

    // 4. 循环删除，直到剩下2个
    while (remaining > 2) {
        // 报数1和2：移动两步（prev和cur同步前进）
        // 注意：要移动 2 次，使得 cur 指向报数 3 的人
        for (int step = 1; step < 3; step++) {
            prev = cur;
            cur = cur->next;
        }
        // 此时 cur 就是报数 3 的人，删除它
        prev->next = cur->next;  // 前驱跳过当前
        free(cur);
        cur = prev->next;        // 从下一个人重新报数
        remaining--;
    }

    // 5. 输出结果（剩下两个节点就在环里）
    printf("\n最后剩余两人: %d 和 %d\n", cur->data, cur->next->data);
    
    return 0;
}