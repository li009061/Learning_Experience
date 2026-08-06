// 设计一个通用的链表，用来存储一个班级中的学生的信息。学生用如下结构体表达：
// struct student
// {
//     char *name;
//     int   age;
// };

//定义一个管理学生的结构体
struct student
{
    char *name;
    int age;
};

#define DATATYPE struct student
#include "_04_1.h"
#include <string.h>

//对比两个学生的数据
bool relative(struct student s1, struct student s2){
    return strcmp(s1.name, s2.name) == 0;
}

//展示链表中的数据
void handle(struct student *data){
    printf("%s %d\n", data->name, data->age);
}

int main(int argc, char const *argv[])
{
    //初始化一条链表
    linklist head = init_linklist();
    if(head == NULL){
        printf("链表初始化失败\n");
        return 1;
    }

    //增加一些新节点
    struct student student1 = {"zhansan", 18};
    struct student student2 = {"lisi", 19};
    struct student student3 = {"wangwu", 20};
    inser_linkNode(head, newNode(student1));
    inser_linkNode(head, newNode(student2));
    inser_linkNode(head, newNode(student3));

    //展示表中的学生信息
    list_ForEach(head, handle);

    //删除指定数据
    delNode(head, relative, student2);
    printf("删除 lisi 后:\n");
    list_ForEach(head, handle);

    //释放链表
    free_linklist(head);

    return 0;
}

