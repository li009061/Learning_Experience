#ifndef BOOK_H
#define BOOK_H

#include "kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern struct list_head book_list;

typedef struct BookNode {
    int book_id; // 图书ID
    char book_name[64];  // 图书名
    char author[64];  // 作者
    char publisher[64];  // 出版社
    int inventory;  // 库存数量
    struct list_head list;  // 链表节点
}Book;


//添加图书
void add_book(void);
//删除图书
void del_book(void);
//查询图书
void cheak_book(void);
//图书信息
void book_info(void);
//精确查找书籍的id和名字
Book * find_book(char * name, bool is_id);


#endif