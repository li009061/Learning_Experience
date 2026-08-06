// 编程实现汉诺塔游戏。

#include <stdio.h>
#include <stdlib.h>

// 栈节点
typedef struct Node {
    int data;           // 盘子大小（数字越大表示盘子越大）
    struct Node *next;
} Node;

// 栈结构（用链表实现，栈顶是链表头）
typedef struct Stack {
    char name;          // 柱子名字 A/B/C
    Node *top;          // 栈顶
    int count;          // 盘子数量
} Stack;

// 初始化栈
Stack *init_stack(char name) {
    Stack *s = malloc(sizeof(Stack));
    s->name = name;
    s->top = NULL;
    s->count = 0;
    return s;
}

// 入栈
void push(Stack *s, int disk) {
    Node *new = malloc(sizeof(Node));
    new->data = disk;
    new->next = s->top;
    s->top = new;
    s->count++;
}

// 出栈
int pop(Stack *s) {
    if (s->top == NULL) {
        printf("错误：%c 柱为空！\n", s->name);
        return -1;
    }
    Node *tmp = s->top;
    int disk = tmp->data;
    s->top = tmp->next;
    free(tmp);
    s->count--;
    return disk;
}

// 获取栈顶元素（不出栈）
int peek(Stack *s) {
    if (s->top == NULL) return -1;
    return s->top->data;
}

// 判断栈是否为空
int is_empty(Stack *s) {
    return s->top == NULL;
}

// 移动盘子：从 src 移动到 dst
void move_disk(Stack *src, Stack *dst) {
    int disk = pop(src);
    if (disk == -1) return;
    
    // 检查：不能大盘压小盘
    if (!is_empty(dst) && peek(dst) < disk) {
        printf("错误：不能将大盘 %d 压在小盘 %d 上！\n", disk, peek(dst));
        push(src, disk);  // 放回去
        return;
    }
    
    push(dst, disk);
    printf("移动盘子 %d: %c → %c\n", disk, src->name, dst->name);
}

// 展示某根柱子
void show_stack(Stack *s) {
    printf("%c: ", s->name);
    if (is_empty(s)) {
        printf("空\n");
        return;
    }
    
    // 从栈底到栈顶打印（需要反转）
    int disks[100], i = 0;
    Node *p = s->top;
    while (p) {
        disks[i++] = p->data;
        p = p->next;
    }
    
    // 从后往前打印（栈底到栈顶）
    for (int j = i - 1; j >= 0; j--) {
        printf("%d ", disks[j]);
    }
    printf("\n");
}

// 展示所有柱子
void show_all(Stack *a, Stack *b, Stack *c) {
    show_stack(a);
    show_stack(b);
    show_stack(c);
    printf("-------------------\n");
}

// 汉诺塔递归算法
// n: 盘子数, src: 源柱, aux: 辅助柱, dst: 目标柱
void hanoi(int n, Stack *src, Stack *aux, Stack *dst) {
    if (n == 1) {
        move_disk(src, dst);
        return;
    }
    
    // 1. 把上面 n-1 个盘子从 src 移到 aux（借助 dst）
    hanoi(n - 1, src, dst, aux);
    
    // 2. 把最底下的盘子从 src 移到 dst
    move_disk(src, dst);
    
    // 3. 把 n-1 个盘子从 aux 移到 dst（借助 src）
    hanoi(n - 1, aux, src, dst);
}

// 初始化：把 n 个盘子放到 A 柱（大盘在下）
void init_hanoi(Stack *a, int n) {
    for (int i = n; i >= 1; i--) {
        push(a, i);   // 先 push n（大盘），后 push 1（小盘）
    }
}

int main(void) {
    int n = 5;  // 盘子数量
    
    Stack *A = init_stack('A');
    Stack *B = init_stack('B');
    Stack *C = init_stack('C');
    
    // 初始化：盘子放到 A 柱
    init_hanoi(A, n);
    
    printf("初始状态：\n");
    show_all(A, B, C);
    
    printf("开始移动：\n");
    hanoi(n, A, B, C);
    
    printf("最终状态：\n");
    show_all(A, B, C);
    
    // 释放内存（略）
    
    return 0;
}