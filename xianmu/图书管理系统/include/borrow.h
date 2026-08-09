#ifndef BORROW_H
#define BORROW_H

#include "user.h"
#include "book.h"
#include "kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


extern struct list_head borrow_list;


typedef struct BorrowNode{
    int book_id; // 图书ID
    int user_id; // 用户ID
    char borrow_time[50]; // 借阅时间
    char return_time[50]; // 归还时间
    struct list_head list; // 链表节点
}BorrowRecord;

//初始化新节点
BorrowRecord * init_borrow(void);
//将借阅记录插入到借阅链表中
void inser_borrowlist(BorrowRecord * new_borrow);
//判断用户输入的是字符串还是数字
bool is_num(char *str);
//借阅图书
void borrow_book(void);
//归还图书
void return_book(void);
//查询借阅记录
void cheak_borrow(void);
//借阅统计
void statistics_borrow(void);

#endif