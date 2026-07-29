// 用双向循环链表存储若干自然数，并将其奇偶重排输出。
// 比如：
// 链表中存储： 1 2 3 4 5 6 7 8 9 …
// 将其重排成： 1 3 5 7 9 … 8 6 4 2 （奇数升序偶数降序）
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    int data;
    struct node *prev;
    struct node *next;
} node;

// 初始化
node *init_list(void) {
    node *head = malloc(sizeof(node));
    if (head != NULL) {
        head->prev = head;
        head->next = head;
    }
    return head;
}

// 尾插
void insert_tail(node *head, node *new) {
    new->next = head;
    new->prev = head->prev;
    head->prev->next = new;
    head->prev = new;
}

// 创建新节点
node *newNode(int data) {
    node *n = malloc(sizeof(node));
    if (n != NULL) {
        n->data = data;
        n->prev = n;
        n->next = n;
    }
    return n;
}

// 判断空
bool isEmpty(node *head) {
    return head == NULL || head->next == head;
}

// 展示
void show_list(node *head) {
    for (node *p = head->next; p != head; p = p->next) {
        printf("%d  ", p->data);
    }
    printf("\n");
}

// 奇偶重排：奇数升序 + 偶数降序
void rearrange(node *head) {
    if (isEmpty(head)) return;
    
    node *odd_head = init_list();    // 奇数链表
    node *even_head = init_list();   // 偶数链表
    
    // 分离奇偶
    node *p = head->next;
    while (p != head) {
        node *next = p->next;  // 保存下一个
        
        // 从原链表摘下
        p->prev->next = p->next;
        p->next->prev = p->prev;
        
        // 插入奇数或偶数链表（头插法，偶数链表自然逆序）
        if (p->data % 2 != 0) {
            // 奇数：尾插保持升序
            insert_tail(odd_head, p);
        } else {
            // 偶数：头插实现逆序
            p->next = even_head->next;
            p->prev = even_head;
            even_head->next->prev = p;
            even_head->next = p;
        }
        
        p = next;
    }
    
    // 拼接：奇数链表 + 偶数链表
    if (!isEmpty(odd_head) && !isEmpty(even_head)) {
        // 奇数尾接偶数头
        odd_head->prev->next = even_head->next;
        even_head->next->prev = odd_head->prev;
        
        // 偶数尾接奇数头（形成循环）
        even_head->prev->next = odd_head;
        odd_head->prev = even_head->prev;
        
        // 更新原头节点
        head->next = odd_head->next;
        head->prev = even_head->prev;
    } else if (!isEmpty(odd_head)) {
        head->next = odd_head->next;
        head->prev = odd_head->prev;
    }
    
    // 释放临时头节点
    free(odd_head);
    free(even_head);
    
    // 重新建立循环
    head->next->prev = head;
    head->prev->next = head;
}

int main(void) {
    node *head = init_list();
    
    for (int i = 1; i <= 9; i++) {
        insert_tail(head, newNode(i));
    }
    
    printf("原始：");
    show_list(head);
    
    rearrange(head);
    
    printf("重排：");
    show_list(head);
    
    return 0;
}