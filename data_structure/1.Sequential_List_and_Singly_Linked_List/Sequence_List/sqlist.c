#include <stdio.h>
#include "sqlist.h"

struct sqlist * sqlist_init(int cap){
    sqlist *sql = malloc(sizeof(sqlist));
    if (sql != NULL)
    {
        sql ->data = calloc(cap , sizeof(int));
        if (sql ->data == NULL)
        {
            free(sql);
            return NULL;
        }
        
        sql ->capacity = cap;
        sql ->last = -1;
    }
    
    return sql;
}

bool add_data(sqlist *sql, int n){
        //1.判断是否还有空间
        if (sql ->last >= sql ->capacity-1)
        {
            return false;
        }
        
        //2.将新数据n放入最后的位置
        sql->last++;
        sql->data[sql->last] = n;   
        return true;
}

bool is_emptu(sqlist *sql){
    return sql ->last == -1;
}

bool del_data(sqlist *sql , int n){
    //判断是否为空
    if (is_emptu(sql))
    {
        return false;
    }
    
    //尝试删除指定数据
    for (int i = 0; i <= sql ->last; i++)
    {
        if (sql ->data[i] == n)
        {
            for (int k = i; k < sql ->last; k++)
            {
                sql ->data[k] = sql ->data[k+1];
            }
            sql -> last--;
            return true;
        }
        
    }
    return false;
}

void output(sqlist *sql){
    if (is_emptu(sql))
    {
        return;
    }
    for (int i = 0; i <= sql ->last; i++)
    {
        printf("%d\t", sql->data[i]);
    }  
    printf("\n");
}


void reverse_output(sqlist *sql){
    if (is_emptu(sql))
    {
        return;
    }
    for (int i = sql ->last; i >=0; i--)
    {
        printf("%d\t", sql->data[i]);
    }  
    printf("\n");
}

void sqlist_destroy(sqlist *sql){
    if (sql == NULL)
    {
        return;
    }
    free(sql ->data);
    free(sql);
}

void swap(int *pa, int *pb){
    int temp;
    temp = *pa;
    *pa = *pb;
    *pb = temp;
}

void sort(sqlist *sql){
    if (is_emptu(sql))
    {
        return;
    }
    //冒泡排序
    int n = sql ->last;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (sql -> data[j] > sql->data[j+1])
            {
                swap(&sql ->data[j], &sql ->data[j+1]);
            }   
        }
    }
}

void sort_better(sqlist *sql){
    if (is_emptu(sql))
    {
        return;
    }
    //冒泡排序
    int n = sql ->last;
    for (int i = 0; i < n; i++)
    {
        int flag = 0;
        for (int j = 0; j < n -i; j++)
        {
            //判断是否逆序，若是则交换他们
            if (sql -> data[j] > sql->data[j+1])
            {
                swap(&sql ->data[j], &sql ->data[j+1]);
                flag = 1;
            }
        }
        if (flag == 0) 
        break;
    }
}