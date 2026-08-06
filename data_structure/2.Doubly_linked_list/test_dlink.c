#include <stdio.h>
#include "dlink_list.h"

int main(int argc, char const *argv[])
{
    
    //初始化一条带头结点的新的链表
    struct node * head = init_list();
    if (head != NULL)
    {
        printf("链表初始化成功\n");
    }else{
        perror("链表初始化失败的原因");
        exit(0);
    }

    //在链表的头部插入一些节点
    for (int i = 1; i <= 5; i++)
    {
        node *new = newNode(i);
        inserhead(head, new);
    }

    // //在链表的尾部部插入一些节点
    // for (int i = 1; i <= 5; i++)
    // {
    //     node *new = newNode(i);
    //     insertail(head, new);
    // }

    //展示链表
    show_list(head);

    //消除节点
    int data1;
    while (1)
    {
        scanf("%d", &data1);

        if (data1 == 0)
        {
            break;
        }

        node * r = find_list(head, data1); 

        if (r == NULL)
        {
            printf("没有找到你要删掉的数值。\n");
        }else{
            free(r);
            remove_list(head);
        }
        
        show_list(head);
    }

    listForEach(head);

    printf("\n");
    
    listForEachPrev(head);
    
    printf("\n");

    destroy(head);

    return 0;
}
