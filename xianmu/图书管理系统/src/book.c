#include "book.h"


//添加图书
void add_book(){
    Book * new_book = calloc(1, sizeof(Book));
    if(new_book == NULL)
        return;

    //将图书信息填入
    printf("请输入书名:\n");
    scanf("%s", new_book->book_name);
    printf("请输入作者名字:\n");
    scanf("%s", new_book->author);
    printf("请输入出版社:\n");
    scanf("%s", new_book->publisher);
    printf("请输入库存数量:\n");
    scanf("%d", &new_book->inventory);

    //寻找到最大的图书的id
    Book *p;
    int max_id = 0;
    list_for_each_entry(p, &book_list, list){
        if(p->book_id > max_id)
            max_id = p->book_id;
    }

    //设置新图书的id为最大的图书的id+1
    new_book->book_id = max_id + 1;

    //将新图书添加到链表尾部
    list_add_tail(&new_book->list, &book_list);
    printf("添加成功\n");
    
    return;
}

//删除图书
void del_book(){
    //按照书的名字来进行删除
    char name[64];
    printf("请输入要删除的书名:\n");
    scanf("%s", name);

    //遍历图书链表，找到要删除的图书
    Book *p;
    list_for_each_entry(p, &book_list, list){
        if(strcmp(p->book_name, name) == 0){
            //判断是否有被借阅，被借阅的书籍无法被删除

            //删除该图书
            list_del_init(&p->list);
            free(p);
            printf("删除成功\n");
            return;
        }
    }

    printf("未找到该图书\n");
    
    return;
}

//查询图书
void cheak_book(){
    //按照书的名字来进行查询
    char name[64];
    printf("请输入要查询的书名(支持模糊查询):\n");
    scanf("%s", name);

    //遍历图书链表，找到要查询的图书
    Book *p;
    int found = 0;
    printf("查询结果:\n");
    list_for_each_entry(p, &book_list, list){
        if(strstr(p->book_name, name) != NULL){
            printf("----------------------\n");
            printf("图书id: %d, 书名: %s, 作者: %s, 出版社: %s, 库存数量: %d\n", p->book_id, p->book_name,  
                p->author, p->publisher, p->inventory);
            found = 1;
        }
    }
    if(found == 0){
        printf("未找到符合条件的图书\n");
    }

    return;
}

void book_info(void){
    printf("----------------------\n");
    printf("图书信息\n");
    printf("----------------------\n");
    Book *p;
    list_for_each_entry(p, &book_list, list){
        printf("图书id: %d, 书名: %s, 作者: %s, 出版社: %s, 库存数量: %d\n",
            p->book_id, p->book_name, p->author, p->publisher, p->inventory);
    }
    printf("\n");
}

//在图书链表中精确查找书籍的id和名字
Book * find_book(char * name, bool is_id){
    Book *p;
    list_for_each_entry(p, &book_list, list){
        if(is_id){
            if(p->book_id == atoi(name))
                return p;
        }else{
            if(strcmp(p->book_name, name) == 0)
                return p;
        }
    }
    return NULL;
}

