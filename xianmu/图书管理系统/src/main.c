#include "kernel_list.h" 
#include "user.h" 
#include "book.h"
#include "borrow.h"
#include "file.h"

//定义一个空链表头
struct list_head user_list;
struct list_head book_list;
struct list_head borrow_list;
User admin = {0, "admin", "123456"};
User *current_user = NULL;

//暂停，等待用户按 Enter 继续
void pause_enther(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    printf("请按 Enter 继续\n");
    while((c = getchar()) != '\n' && c != EOF);
}

//初始化系统链表
void init(void){
    //初始化用户链表
    INIT_LIST_HEAD(&user_list);
    //初始化图书链表
    INIT_LIST_HEAD(&book_list);
    //初始化借阅链表
    INIT_LIST_HEAD(&borrow_list);

}

//读取信息
void load_data(void){
    //读取用户信息
    load_userdata();
    //读取图书信息
    load_bookdata();
    //读取借阅信息
    load_borrowdata();
}

//写入信息
void write_data(void){
    //写入用户信息
    write_userdata();
    //写入图书信息
    write_bookdata();
    //写入借阅信息
    write_borrowdata();
}

//图书管理菜单
void admin_book_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf("图书管理菜单\n");
        printf("----------------------\n");
        printf("1.添加图书\n");
        printf("2.删除图书\n");
        printf("3.查询图书\n");
        printf("4.退出\n");
        printf("----------------------\n");
        printf("请输入您的选择(1-4)：");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-4)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
            add_book();
            pause_enther();
            break;
            case 2:
            del_book();
            pause_enther();
            break;
            case 3:
            cheak_book();
            pause_enther();
            break;
            case 4:
            printf("返回主菜单\n");
            return;
            default:
            printf("输入错误，请重新输入(1-4)\n");
            sleep(1);
            continue;
        }
    }
}



//用户管理菜单
void admin_user_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf("用户管理菜单\n");
        printf("----------------------\n");
        printf("1.添加用户\n");
        printf("2.删除用户\n");
        printf("3.查询用户\n");
        printf("4.退出\n");
        printf("----------------------\n");
        printf("请输入您的选择(1-4)：");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-4)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
            add_user();
            pause_enther();
            break;
            case 2:
            del_user();
            pause_enther();
            break;
            case 3:
            cheak_user();
            pause_enther();
            break;
            case 4:
            printf("返回主菜单\n");
            return;
            default:
            printf("输入错误，请重新输入(1-4)\n");
            sleep(1);
            continue;
        }
    }
}

//管理员菜单
void admin_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf("管理员菜单\n");
        printf("----------------------\n");
        printf("1.用户管理\n");
        printf("2.图书管理\n");
        printf("3.图书信息\n");
        printf("4.借阅统计\n");
        printf("5.退出\n");
        printf("----------------------\n");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-5)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
            admin_user_menu();
            break;
            case 2:
            admin_book_menu();
            break;
            case 3:
            book_info();
            pause_enther();
            break;
            case 4:
            statistics_borrow();
            pause_enther();
            break;
            case 5:
            printf("返回主菜单\n");
            return;
            default:
            printf("输入错误，请重新输入(1-5)\n");
            sleep(1);
            continue;
        }
    }
}

//用户菜单
void user_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf("用户菜单\n");
        printf("----------------------\n");
        printf("1.借阅图书\n");
        printf("2.还书图书\n");
        printf("3.查询借阅记录\n");
        printf("4.查询图书\n");
        printf("5.退出\n");
        printf("----------------------\n");
        printf("请输入您的选择(1-5)：");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-5)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
            borrow_book();
            pause_enther();
            break;
            case 2:
            return_book();
            pause_enther();
            break;
            case 3:
            cheak_borrow();
            pause_enther();
            break;
            case 4:
            cheak_book();
            pause_enther();
            break;
            case 5:
            printf("返回主菜单\n");
            return;
            default:
            printf("输入错误，请重新输入(1-5)\n");
            sleep(1);
            continue;
        }
    }
}

//访客菜单
void visitor_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf("访客菜单\n");
        printf("----------------------\n");
        printf("1.图书信息\n");
        printf("2.查询图书\n");
        printf("3.退出\n");
        printf("----------------------\n");
        printf("请输入您的选择(1-3)：");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-3)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
            book_info();
            pause_enther();
            break;
            case 2:
            cheak_book();
            pause_enther();
            break;
            case 3:
            printf("返回主菜单\n");
            return;
            default:
            printf("输入错误，请重新输入(1-3)\n");
            sleep(1);
            continue;
        }
    }
}

//主菜单
void main_menu(void){
    int choose;
    while(1){
        printf("----------------------\n");
        printf(" 欢迎来到图书管理系统 \n");
        printf("----------------------\n");
        printf("    ===主菜单===    \n");
        printf("1.登录\n");
        printf("2.注册\n");
        printf("3.访客登录\n");
        printf("4.退出\n");
        printf("----------------------\n");
        printf("请输入您的选择(1-4)：");
        if(scanf("%d", &choose) != 1){
            if(feof(stdin)){            // ← 新增：输入流结束
                printf("输入结束，退出菜单\n");
                return;                 // ← 新增：直接退出当前菜单函数
            }
            printf("输入错误，请重新输入(1-4)\n");
            sleep(1);
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch(choose){
            case 1:
                int ret = login_user();
                if(ret == 1)
                    admin_menu();
                else if(ret == 2)
                    user_menu();
                break;
            case 2:
                register_user();
                break;
            case 3:
                visitor_menu();
                break;
            case 4:
                printf("感谢您的使用，再见！\n");
                return;
            default:
                printf("输入错误，请重新输入(1-4)\n");
                sleep(1);
                continue;
        }
    }
}


int main(void)
{
    //初始化系统
    init();
    
    //读取信息
    load_data();

    //登录系统
    main_menu();

    //写入信息
    write_data();
    return 0;
}
