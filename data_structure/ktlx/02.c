// 创建一个单向表，并从键盘接收数字输入，将输入的正整数按从小到大的顺序插入链表，
// 并在输入负整数的时候将其绝对值数据删除。每次输入后，将链表的内容打印到屏幕上。

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//管理单向表结构体
typedef struct node{
    int data;
    struct node *next;
}linklist;

//初始化单链表(init_linklist)
linklist * init_linklist(){
    linklist *head = calloc(1, sizeof(linklist));
    if (head == NULL)
        return NULL;

    head->next = NULL;

    return head;
}

//判断是否为空(is_Empty)
bool is_Empty(linklist *head){
    return (head->next == NULL);
}

//定义一个新节点newNode
linklist * newNode(int new){
    linklist *link = calloc(1, sizeof(linklist));
    if (link == NULL)
        return NULL;

    link->data = new;
    link->next = NULL;
    return link;
}

//将新节点从小到大添加到链表中inser_linklist
void inser_linklist(linklist *head, linklist *new){
    if (is_Empty(head)){
        head->next = new;
        return;
    }
    
    linklist *p, *p1;
    for(p = head->next, p1 = head; p != NULL; p = p->next){
        if(new->data < p->data){
            break;
        }
        p1 = p;
    }
    new->next = p;
    p1->next = new;
}

//将指定的节点删除del_linklist
bool del_linklist(linklist *head, int data){
    if (is_Empty(head))
        return false;

    linklist *p, *p1;
    for(p = head->next, p1 = head; p != NULL;p1 = p, p = p->next){
        if(p->data == data){
            p1->next = p->next;
            free(p);
            return true;
        }
    }

    return false;
}

//展示链表show_linklist
void show_linklist(linklist *head){
    if(is_Empty(head))
        return;

    linklist *p;
    for(p = head->next; p != NULL; p = p->next){
        printf("%d\t", p->data);
    }
    printf("\n");
}

//释放链表free_linklist
void free_linklist(linklist *head){
    if(is_Empty(head)){
        free(head);
        return;
    }

    linklist *p;
    for(p = head->next; p != NULL; p = head->next){
        head->next = p->next;
        free(p);
    }
    free(head);
}

int main(int argc, char const *argv[])
{
    //初始化一个新的链表
    linklist *head = init_linklist();
    if (head == NULL){
        perror("链表初始化失败");
        return 1;
    }

        printf("链表初始化成功！\n");

    //从键盘中读取数字
    int data;
    while (1)
    {
        if(scanf("%d", &data) == 1){
            if(data == 0)
                break;
            //将数字添加到链表中
            if (data > 0)
            {
                inser_linklist(head, newNode(data));
                show_linklist(head);
            }else{
                if(is_Empty(head)){
                    printf("单链表为空，无法删除。\n");
                    continue;
                }
                //如果输入负数删掉其绝对值的数
                del_linklist(head, abs(data));
                show_linklist(head);
            }
        }else{
            while(getchar() != '\n');
            break;
        }

    }

    //释放链表
    free_linklist(head);

    return 0;
}
