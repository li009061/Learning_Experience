#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int id;             // 编号 1~n
    struct node *next;
} node;

// 创建 n 个人的循环链表
node *create_circle(int n) {
    node *head = malloc(sizeof(node));
    head->id = 1;
    
    node *tail = head;
    for (int i = 2; i <= n; i++) {
        node *new = malloc(sizeof(node));
        new->id = i;
        tail->next = new;
        tail = new;
    }
    tail->next = head;   // 尾指向头，形成循环
    
    return head;
}

// 删除第 m 个节点，返回下一个起始节点
// prev 是待删除节点的前一个
node *kill(node *prev) {
    node *victim = prev->next;
    prev->next = victim->next;   // 跳过 victim
    printf("%d 自杀\n", victim->id);
    free(victim);
    return prev->next;            // 从下一个开始报数
}

// 约瑟夫环：报数到 3 自杀，剩 2 人
void josephus(int n) {
    node *head = create_circle(n);
    node *p = head;              // 当前报数的人
    
    // 找到 p 的前一个（用于删除）
    // 循环链表中找前一个：遍历到 p->next == p 的前一个
    // 初始时找 head 的前一个（即尾节点）
    node *prev = head;
    while (prev->next != head) {
        prev = prev->next;
    }
    
    // 当只剩 2 人时停止
    // 判断条件：p->next->next == p（3人）或 p->next == prev（2人）
    // 更简单：计数节点数，或判断 p->next != p 且 p->next->next != p
    while (prev->next != prev && prev->next->next != prev) {
        // 报数 1, 2：移动 prev 和 p 两次
        // prev 是报数 1 的人的前一个，p 是报数 1 的人
        // 实际上 prev 和 p 应该同步移动
        
        // 报数 1
        prev = prev->next;   // prev 指向报数 1 的人
        p = p->next;         // p 指向报数 2 的人？不对...
        
        // 重新理清：prev 始终指向当前报数者的前一个
        // 报数 1：prev->next 报 1
        // 报数 2：prev->next->next 报 2  
        // 报数 3：prev->next->next->next 报 3，删除它
    }
}

// /简化版核心逻辑
node *josephus(int n, int m, int remain) {
    // n 人, 报数到 m 自杀, 剩 remain 人
    node *head = create_circle(n);
    
    node *prev = head;
    while (prev->next != head) prev = prev->next;  // prev 指向尾
    
    int count = n;
    while (count > remain) {
        // 报数 1 到 m-1：prev 移动 m-1 次
        for (int i = 0; i < m - 1; i++) {
            prev = prev->next;
        }
        // 删除 prev->next（报数到 m 的人）
        node *victim = prev->next;
        prev->next = victim->next;
        printf("%d 自杀\n", victim->id);
        free(victim);
        count--;
    }
    
    return prev->next;  // 返回剩下的头节点
}