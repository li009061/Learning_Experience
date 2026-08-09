// 编写一个程序，产生若干长度与内容都随机的字符串，将字符串存入哈希表中，并在程序中提供查表算法演示。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 存储字符串的哈希表节点
struct node {
    char *data;          // 字符串
    struct node *next;
};

// 哈希表管理结构体
typedef struct {
    unsigned long table_size;
    struct node **table_entry;
} hash_table;

// 初始化哈希表
hash_table *init_hash_table(unsigned long size) {
    hash_table *ht = malloc(sizeof(hash_table));
    if (!ht) return NULL;
    ht->table_size = size;
    ht->table_entry = calloc(size, sizeof(struct node *));
    if (!ht->table_entry) {
        free(ht);
        return NULL;
    }
    return ht;
}

// BKDR 哈希函数（字符串）
unsigned long hash_string(const char *str, unsigned long size) {
    unsigned long seed = 131;
    unsigned long hash = 0;
    while (*str) {
        hash = hash * seed + (unsigned char)(*str++);
    }
    return hash % size;
}

// 插入字符串（拷贝副本）
void hash_insert(hash_table *ht, const char *str) {
    unsigned long idx = hash_string(str, ht->table_size);
    struct node *new_node = malloc(sizeof(struct node));
    if (!new_node) return;
    new_node->data = strdup(str);   // 拷贝字符串
    if (!new_node->data) {
        free(new_node);
        return;
    }
    // 头插法
    new_node->next = ht->table_entry[idx];
    ht->table_entry[idx] = new_node;
}

// 查找字符串
int hash_search(hash_table *ht, const char *str) {
    unsigned long idx = hash_string(str, ht->table_size);
    struct node *p = ht->table_entry[idx];
    while (p) {
        if (strcmp(p->data, str) == 0)
            return 1;
        p = p->next;
    }
    return 0;
}

// 展示某个桶（索引）
void show_bucket(hash_table *ht, unsigned long idx) {
    if (idx >= ht->table_size) {
        printf("索引超出范围\n");
        return;
    }
    struct node *p = ht->table_entry[idx];
    if (!p) {
        printf("桶 %lu 为空\n", idx);
        return;
    }
    printf("桶 %lu: ", idx);
    while (p) {
        printf("\"%s\" -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

// 打印整个哈希表（所有桶）
void show_all(hash_table *ht) {
    for (unsigned long i = 0; i < ht->table_size; i++) {
        if (ht->table_entry[i]) {
            show_bucket(ht, i);
        }
    }
}

// 释放哈希表（释放所有节点和字符串）
void free_hash_table(hash_table *ht) {
    for (unsigned long i = 0; i < ht->table_size; i++) {
        struct node *p = ht->table_entry[i];
        while (p) {
            struct node *tmp = p;
            p = p->next;
            free(tmp->data);
            free(tmp);
        }
    }
    free(ht->table_entry);
    free(ht);
}

// 生成随机字符串（长度 1~20，可打印 ASCII）
char *generate_random_string(void) {
    int len = rand() % 20 + 1;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    for (int i = 0; i < len; i++) {
        // 可打印 ASCII 33~126
        str[i] = (char)(rand() % 94 + 33);
    }
    str[len] = '\0';
    return str;
}

int main() {
    srand(time(NULL));

    // 初始化哈希表（大小 101，质数）
    hash_table *ht = init_hash_table(101);
    if (!ht) {
        printf("哈希表初始化失败\n");
        return 1;
    }

    // 产生 10 个随机字符串并插入
    printf("产生的字符串:\n");
    for (int i = 0; i < 100; i++) {
        char *s = generate_random_string();
        printf("%d: %s\n", i+1, s);
        hash_insert(ht, s);
        free(s);   // 释放原字符串，因为哈希表内部已有副本
    }

    // 展示整个哈希表
    printf("\n哈希表内容（含冲突链）:\n");
    show_all(ht);

    // 查表演示
    char target[100];
    while (1)
    {
        printf("\n请输入要查找的字符串: ");
        scanf("%s", target);
        if(target == "abcd")
            break;
        if (hash_search(ht, target)) {
            printf("找到了！\n");
        } else {
            printf("没找到。\n");
        }

    }
    

    // 释放哈希表
    free_hash_table(ht);

    return 0;
}