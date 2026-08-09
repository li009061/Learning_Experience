#include "borrow.h"

//初始化新节点
BorrowRecord *init_borrow(){
    BorrowRecord * new_borrow = calloc(1, sizeof(BorrowRecord));
    if(new_borrow == NULL)
        return NULL;

    return new_borrow;
}

//判断用户输入的是字符串还是数字
bool is_num(char * borrow){
    if(borrow == NULL)
        return false;

    for(int i = 0; borrow[i] != '\0'; i++){
        if(borrow[i] < '0' || borrow[i] > '9')
            return false;
    }

    return true;
}

//将借阅记录插入到借阅链表中
void inser_borrowlist(BorrowRecord * new_borrow){
    if(new_borrow == NULL)
        return;

    list_add_tail(&(new_borrow->list), &borrow_list);
    return;
}

//借阅图书
void borrow_book(void){
    BorrowRecord *new_borrow = init_borrow();
    if(new_borrow == NULL)
        return;

    printf("-----------------------\n");
    printf("借阅图书\n");
    printf("-----------------------\n");
    Book *s;
    list_for_each_entry(s, &book_list, list){
        printf("图书ID:%d, 图书名称:%s, 作者:%s, 出版社:%s, 库存:%d\n", 
            s->book_id, s->book_name, s->author, s->publisher, s->inventory);
    }

    printf("请输入您要借阅的图示id或者名称：\n");
    char borrow[64];
    scanf("%s", borrow);

    // 判断用户输入的是字符串还是数字
    bool is_number = is_num(borrow);

    //寻找书籍
    Book *p = find_book(borrow, is_number);
    if(p == NULL){
        printf("未找到该图书\n");
        free(new_borrow);
        return;
    }

    if(p->inventory == 0){
        printf("该图书库存不足\n");
        free(new_borrow);
        return;
    }

    // 借书成功：填记录 → 减库存 → 挂链表
    new_borrow->book_id = p->book_id;
    new_borrow->user_id = current_user->id;
    time_t now = time(NULL);
    strcpy(new_borrow->borrow_time, ctime(&now));
    p->inventory--;
    inser_borrowlist(new_borrow);
    printf("您要借阅的图书是%s, 借阅成功\n", p->book_name);

    return;
}

//归还图书
void return_book(void){
    printf("-----------------------\n");
    printf("归还图书\n"); 
    printf("-----------------------\n");
    printf("请输入您要归还的图示id：\n");
    char borrow[64];
    scanf("%s", borrow);

    //寻找借阅记录
    Book *book = find_book(borrow, is_num(borrow));
    if(book == NULL){
        printf("未找到该图书\n");
        return;
    }

    //归还书籍
    BorrowRecord *rec;
    list_for_each_entry(rec, &borrow_list, list){
        if(rec->book_id == book->book_id && rec->user_id == current_user->id
            && rec->return_time[0] == '\0'){
                time_t now = time(NULL);
                strcpy(rec->return_time, ctime(&now));
                book->inventory++;
                printf("归还成功\n");
                return;
            }
    }

    printf("您还没有借阅该图书的记录\n");
}

//查询借阅记录
void cheak_borrow(void){
    printf("-----------------------\n");
    printf("查询借阅记录\n");
    printf("-----------------------\n");
    printf("您借阅的图书有：\n");
    BorrowRecord *rec;
    list_for_each_entry(rec, &borrow_list, list){
        if(rec->user_id == current_user->id){
            Book *book;
            list_for_each_entry(book, &book_list, list){
                if(book->book_id == rec->book_id){
                    printf("图书id:%d, 书名:%s, 作者:%s, 出版社:%s\n", 
                        book->book_id, book->book_name, book->author, book->publisher);
                    printf("借阅时间:%s", rec->borrow_time);
                    if(rec->return_time[0] == '\0')
                        printf("  状态: 借阅中\n");
                    else
                        printf("  状态: 已还 (%s)", rec->return_time);
                }
            }
        }
    }
    return;
}

void statistics_borrow(void){
    int total = 0, unreturned = 0;  //统计总借阅数和未还数
    printf("-----------------------\n");
    printf("借阅统计\n");
    printf("-----------------------\n");
    BorrowRecord *p;
    list_for_each_entry(p, &borrow_list, list){
        printf("图书id:%d, 用户id:%d, 借阅时间:%s\n",
             p->book_id, p->user_id, p->borrow_time);
        total++;
        if(p->return_time[0] == '\0')
            unreturned++;
    }

    printf("总借阅数:%d, 未还数:%d\n", total, unreturned);
    
    return;
}