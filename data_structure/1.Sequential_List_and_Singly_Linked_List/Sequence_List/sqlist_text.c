#include "sqlist.h"

int main(int argc, char const *argv[])
{
    sqlist *sql = sqlist_init(10);

    printf("请输入若干个整数（输入非整数就退出）\n");
    int n;
    while (1)
    {
        if (scanf("%d", &n) != 1)
        {
            break;
        }
        add_data(sql, n);
    }

    printf("原始数据（正序）：\n");
    output(sql);

    // 排序并打印
    sort(sql);
    printf("排序后（升序）：\n");
    output(sql);


    printf("原始数据（逆序）：\n");
    reverse_output(sql);

    sort_better(sql); 
    printf("排序后：\n");
    output(sql);

    sqlist_destroy(sql);
    return 0;
}