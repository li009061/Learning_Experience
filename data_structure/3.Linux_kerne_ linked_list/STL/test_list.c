struct student{
    unsigned int ID;
    int age;
    char *name;
};

#define DATATYPE struct student
#include "list.h"

void showinfo(struct student *p){
    printf("姓名:%s,年龄:%d\n", p->name, p->age);
}

//判断两个节点是否一样
bool equal(struct student a, struct student b){
    return a.ID == b.ID;
}

int main(int argc, char const *argv[])
{
    //默认链表存储整数
    linklist head = initList();

    struct student Jack = {1, 21, "Jack"};
    struct student Rose = {2, 22, "Rose"};
    struct student Bill = {3, 18, "bill"};

    insertail(head, newNode(Jack));
    insertail(head, newNode(Rose));
    insertail(head, newNode(Bill));


    //遍历整个链表。并针对每个节点调用showinfo来看信息
    listForEach(head, showinfo);

    linklist p = findNode(head, Jack , equal);
    if (p != NULL)
    {
        printf("找到了%s!\n", p->data.name);
        remove_list(p);
        free(p);
    }else{
        printf("找不到\n");
    }
    
    p = findNode(head, Jack , equal);
    if (p != NULL)
    {
        printf("找到了%s!\n", p->data.name);
        remove_list(p);
        free(p);
    }else{
        printf("找不到\n");
    }
    
    listForEach(head, showinfo);
    return 0;
}
