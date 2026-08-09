#ifndef FILE_H
#define FILE_H

#include "book.h"
#include "borrow.h"
#include "kernel_list.h"
#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

extern struct list_head user_list;
extern struct list_head book_list;
extern struct list_head borrow_list;
extern User admin;  //只声明，不定义

//读取用户信息
void load_userdata(void);
//读取图书信息
void load_bookdata(void);
//读取借阅信息
void load_borrowdata(void);
//写入用户信息
void write_userdata(void);
//写入图书信息
void write_bookdata(void);
//写入借阅信息
void write_borrowdata(void);



#endif