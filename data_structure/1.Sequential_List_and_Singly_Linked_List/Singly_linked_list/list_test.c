#include <stdio.h>
#include "list.h"

int main(int argc, char const *argv[])
{
    //1.搞个空链表
    node * mylist = list_init();

    //2.放入若干个整数
    int n;
    scanf("%d", &n);
    for (int i = 0; i <= n; i++)
    {
        //2.1 获得一个新节点
        node *new = new_node(i);

        //2.2 将新节点，链入链表的头部
        // add_node(mylist, new);
        add_node_tail(mylist ,new);
    }
    
    //3.遍历链表，输出各个节点数据
    list_for_each(mylist);

    //4.销毁链表。释放内存
    list_destroy(mylist);

    return 0;
}
