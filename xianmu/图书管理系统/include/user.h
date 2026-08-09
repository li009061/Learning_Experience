#ifndef USER_H
#define USER_H

#include "kernel_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct UserNode {
    int id; // 用户ID
    char name[64];  // 用户名
    char password[20];  // 密码
    struct list_head list;  // 链表节点
}User;


extern struct list_head user_list;
extern User admin;
extern User *current_user;

//登录用户
int login_user(void);
//注册用户
bool register_user(void);
//初始化用户
User * init_user(void);
//将用户添加到用户链表中
void inser_userlist(User * new_user);
//遍历数据判断用户信息是否存在
User *cheak_user_info(char * username, char * password);
//添加用户
void add_user(void);
//删除用户
void del_user(void);
//查询用户
void cheak_user(void);




#endif