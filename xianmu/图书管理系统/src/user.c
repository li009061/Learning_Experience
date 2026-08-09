#include "user.h"


//初始化用户
User * init_user(void){
    User * new_user = calloc(1, sizeof(User));
    if(new_user == NULL)
        return NULL;

    return new_user;
}

//将用户添加到用户链表中
void inser_userlist(User * new_user){
    if(new_user == NULL)
        return;

    int max_id = 0;
    User * user;
    list_for_each_entry(user, &user_list, list){
        if(user->id > max_id)
            max_id = user->id;
    }
    new_user->id = max_id + 1;
    list_add_tail(&(new_user->list), &user_list);

    return;
}

//遍历数据判断用户信息是否存在
User *cheak_user_info(char * username, char * password){
    User * user;
    list_for_each_entry(user, &user_list, list){
        if(strcmp(user->name, username) == 0 && strcmp(user->password, password) == 0){
            return user;
        }
    }

    return NULL;
}

//遍历数据判断用户名是否存在
User * cheak_user_name(char * name){
   User * user;
    list_for_each_entry(user, &user_list,list){
        if(strcmp(user->name, name) == 0){
                return user;
        }
    }

    return NULL;
}

//登录用户
int login_user(void){
    char username[64];
    char password[20];
    printf("\n---------------------------\n");
    printf("当前正在登录用户中\n");
    printf("---------------------------\n");
    printf("请输入您的用户名(不要超过15个字符):\n\n");
    scanf("%s",username);
    printf("请输入您的密码:\n\n");
    scanf("%s",password);
    if (strcmp(username, admin.name)==0 && strcmp(password, admin.password)==0){
        printf("管理员登录成功\n");
        return 1;
    }

    User * user = cheak_user_info(username,password);
    if(user){
        current_user = user;
        printf("登录成功\n");
        return 2;
    }else{
        printf("用户名不存在或者密码错误!请重新登录\n");
        return 0;
    }
}

//注册用户
bool register_user(void){
    User * new_user = init_user();
    if(new_user == NULL){
        perror("内存分配失败");
        return false;
    }
    printf("---------------------------\n");
    printf("当前正在注册用户中\n");
    printf("---------------------------\n");
    printf("请输入您的用户名(不要超过15个字符):\n\n");
    scanf("%s",new_user->name);
    printf("请输入您的密码:\n\n");
    scanf("%s",new_user->password);
    if(cheak_user_name(new_user->name)){
        printf("用户名已存在\n");
        free(new_user);
        return false;
    }

    inser_userlist(new_user);
    printf("注册成功\n");

    return true;
}


//添加用户
void add_user(void){
    User * new_user = init_user();
    if(new_user == NULL)
        return;

    printf("---------------------------\n");
    printf("当前正在添加用户中\n");
    printf("---------------------------\n");
    printf("请输入您要创建的用户名:\n");
    scanf("%s",new_user->name);
    printf("请输入您要创建的用户的密码:\n");
    scanf("%s", new_user->password);
    if(cheak_user_name(new_user->name)){
        printf("用户名已存在\n");
        free(new_user);
        return;
    }

    inser_userlist(new_user);
    printf("添加成功\n");

    return;
}

//删除用户
void del_user(void){
    printf("---------------------------\n");
    printf("当前正在删除用户中\n");
    printf("---------------------------\n");
    printf("请输入您要删除的用户名:\n");
    char name[64];
    scanf("%s", name);
    User * user;
    list_for_each_entry(user, &user_list,list){
        if(strcmp(user->name, name) == 0){
            //检查该用户是否有借阅记录(如果有借阅情况，给管理员自己判断能不能删除)

            //删除该用户
            list_del_init(&user->list);
            free(user);
            printf("删除成功\n");
            return;
        }
    }
    printf("未找到该用户\n");
}

//查询用户
void cheak_user(void){
    printf("---------------------------\n");
    printf("当前正在查询用户中\n");
    printf("---------------------------\n");
    printf("请输入您要查询的用户名:\n");
    char name[64];
    scanf("%s", name);
    User * user = cheak_user_name(name);
    if(user){
             printf("----------------------\n");
            printf("用户ID:%d, 用户名:%s, 密码:%s\n", 
                user->id, user->name, user->password);
                return;
    }

    printf("未找到该用户\n");
    return;
}
