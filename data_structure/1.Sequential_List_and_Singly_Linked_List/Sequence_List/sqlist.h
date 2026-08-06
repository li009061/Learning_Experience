#ifndef __SQLIST_H
#define __SQLIST_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//顺序表管理结构体类型定义
typedef struct sqlist
{
    int *data;
    int capacity;
    int last;
}sqlist;

//顺序表的操作接口声明
struct sqlist * sqlist_init(int cap /*初始容量大小*/);
bool add_data(sqlist *sql , int n);
bool del_data(sqlist *sql , int n);
void output(sqlist *sql);
bool is_emptu(sqlist *sql);
void reverse_output(sqlist *sql);
void sqlist_destroy(sqlist *sql);
void sort(sqlist *sql);
void swap(int *pa, int *pb);
void sort_better(sqlist *sql);
#endif