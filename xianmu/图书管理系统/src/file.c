#include "file.h"

//读取用户信息
void load_userdata(){
    //读取用户信息
    FILE *fp = fopen("data/user.txt", "r");
    if(fp == NULL)
        return;

    char line[256];

    if(fgets(line,sizeof(line), fp) == NULL){
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = '\0';
        User *p = calloc(1, sizeof(User));
        if(p == NULL)
            continue;

        sscanf(line, "%d, %[^,], %s",&p->id,p->name,p->password);

        list_add_tail(&p->list, &user_list);
    }

    fclose(fp);
    printf("用户信息读取成功!\n");
}

//读取图书信息
void load_bookdata(void){
    //读取图书信息
    FILE *fp = fopen("data/book.txt", "r");
    if(fp == NULL)
        return;

    char line[256];

    //读取第一行数据，跳过表头
    if(fgets(line, sizeof(line), fp) == NULL){
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = '\0';
        //也可以用line[strlen(line) - 1] = '\0'来代替
        //strcspn的作用L计算一个字符串中，从开头起有多少个字符 不包含 在指定的拒绝字符集中
        Book *p = calloc(1, sizeof(Book));
        if(p == NULL)
            continue;

        sscanf(line, "%d, %[^,], %[^,], %[^,], %d",
            &p->book_id, p->book_name, p->author, p->publisher, &p->inventory);

        //将图书信息写入图书链表
        list_add_tail(&p->list, &book_list);
    }

    fclose(fp);
    printf("图书信息读取成功!\n");  
}


//读取借阅信息
void load_borrowdata(){
    //读取借阅信息
    FILE *fp = fopen("data/borrow.txt", "r");
    if(fp == NULL)
        return;

    char line[256];

    if(fgets(line,sizeof(line), fp) == NULL){
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = '\0';
        BorrowRecord *p = calloc(1, sizeof(BorrowRecord));
        if(p == NULL)
            continue;

        sscanf(line, "%d, %d, %[^,], %[^,]", &p->book_id, &p->user_id, p->borrow_time, p->return_time);

        list_add_tail(&p->list, &borrow_list);
    }

    fclose(fp);
    printf("借阅信息读取成功!\n");
}



//写入用户信息
void write_userdata(){
    //写入用户信息
    FILE *fp = fopen("data/user.txt", "w");
    if(fp == NULL)
        return;

    fprintf(fp, "用户ID,用户名,密码\n");

    User *p;
    list_for_each_entry(p, &user_list, list){
        fprintf(fp, "%d, %s, %s\n", p->id, p->name, p->password);
    }

    fclose(fp);
    printf("用户信息写入成功!\n");
}

//写入书籍信息
void write_bookdata(void){
    //写入图书信息
    FILE *fp = fopen("data/book.txt", "w");
    if(fp == NULL)
        return;

    fprintf(fp, "编号,书名,作者,出版社,剩余本数\n");

    Book *p;
    list_for_each_entry(p, &book_list, list){
        fprintf(fp, "%d, %s, %s, %s, %d\n", 
            p->book_id, p->book_name, p->author, p->publisher, p->inventory);
    }

    fclose(fp);
    printf("图书信息写入成功!\n");
}

//写入借阅信息
void write_borrowdata(void){
    //写入借阅信息
    FILE *fp = fopen("data/borrow.txt", "w");
    if(fp == NULL)
        return;

    fprintf(fp, "图书ID,用户ID,借阅时间,归还时间\n");

    BorrowRecord *p;
    list_for_each_entry(p, &borrow_list, list){
        char bt[50], rt[50];
        strcpy(bt, p->borrow_time);                 //因为time_now = time(NULL)
        bt[strcspn(bt, "\n")] = '\0';               //strcpy(....,ctime(&now))----ctime() 生成的借书时间末尾自带 \n（"Sun Aug 9 17:24:52 2026\n"）
        strcpy(rt, p->return_time);                 //所以要切掉\n字符
        rt[strcspn(rt, "\n")] = '\0';  
        fprintf(fp, "%d, %d, %s, %s\n", p->book_id, p->user_id, bt, rt);
    }

    fclose(fp);
    printf("借阅信息写入成功!\n");
}