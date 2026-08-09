#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 20
#define HASH_PRIME 19   // 质数，减少冲突

typedef int datatype;

struct node
{
    datatype data;
    struct node *next;
};

typedef struct
{
    unsigned long table_size;
    struct node **table_entry;
} hash_table;

// 统一哈希函数
unsigned long hash_func(datatype data) {
    return data % HASH_PRIME;
}

hash_table *init_ht(unsigned long size)
{
    hash_table *ht = malloc(sizeof(hash_table));
    ht->table_size = size;
    ht->table_entry = calloc(size, sizeof(struct node *));
    return ht;
}

// 普通展示
void show_ht(hash_table *ht)
{
    for (int i = 0; i < ht->table_size; i++)
    {
        printf("table_entry[%d]: ", i);
        struct node *p = ht->table_entry[i];
        
        while (p != NULL)
        {
            printf("%d -> ", p->data);
            p = p->next;
        }
        printf("NULL\n");
    }
}

// 插入时展示（标记插入位置）
void show_insert(hash_table *ht, unsigned long pos, datatype data)
{
    for (int i = 0; i < ht->table_size; i++)
    {
        printf("table_entry[%d]: ", i);
        struct node *p = ht->table_entry[i];
        
        while (p != NULL)
        {
            printf("%d -> ", p->data);
            p = p->next;
        }
        printf("NULL");
        
        if (pos == i) {
            printf("  <-- 插入 %d", data);
        }
        printf("\n");
    }
    printf("=================================\n");
}

void hash_add(datatype data, hash_table *ht)
{
    unsigned long hash_addr = hash_func(data);
    
    struct node *new = malloc(sizeof(struct node));
    new->data = data;
    new->next = NULL;
    
    show_insert(ht, hash_addr, data);
    
    if (ht->table_entry[hash_addr] == NULL)
    {
        ht->table_entry[hash_addr] = new;
    }
    else
    {
        struct node *p = ht->table_entry[hash_addr];
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = new;
    }
}

void hash_search(datatype data, hash_table *ht)
{
    unsigned long find = hash_func(data);
    struct node *p = ht->table_entry[find];
    int step = 0;
    
    while (p != NULL)
    {
        step++;
        if (p->data == data)
        {
            printf("找到 %d，位于 table_entry[%lu]，比较 %d 次\n", 
                   data, find, step);
            return;
        }
        p = p->next;
    }
    
    printf("未找到 %d，位于 table_entry[%lu]，比较 %d 次\n", 
           data, find, step);
}

// 释放内存
void destroy_ht(hash_table *ht)
{
    for (int i = 0; i < ht->table_size; i++)
    {
        struct node *p = ht->table_entry[i];
        while (p != NULL)
        {
            struct node *tmp = p;
            p = p->next;
            free(tmp);
        }
    }
    free(ht->table_entry);
    free(ht);
}

int main(void)
{
    hash_table *ht = init_ht(SIZE);
    
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        hash_add(rand() % 1000, ht);
    }
    
    printf("\n最终哈希表：\n");
    show_ht(ht);
    
    printf("\n请输入你要查找的数（-1退出）:\n");
    int m;
    while (1)
    {
        if (scanf("%d", &m) != 1)
        {
            printf("输入错误，请重新输入整数\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (m == -1) break;
        
        hash_search(m, ht);
    }
    
    destroy_ht(ht);
    return 0;
}