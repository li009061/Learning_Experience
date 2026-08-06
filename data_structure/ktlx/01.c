// 创建一个顺序表，并从键盘接收数字输入，将输入的正整数按从小到大的顺序插入顺序表，
// 并在输入负整数的时候将其绝对值数据删除。每次输入后，将顺序表的内容打印到屏幕上。
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//顺序表的一个结构体
typedef struct list{
    int cap;
    int last;
    int *data;
}list;

//初始化顺序表(init)
list * init(int cap){
    list * list = malloc(sizeof(list));
    if (list != NULL)
    {
        list->last = -1;
        list->cap = cap;
        list->data = calloc(cap, sizeof(int));
        if (list->data == NULL)
        {
            free(list);
            return NULL;
        }
    }else
        return NULL;

    return list;
}

//判断是否为空(is_Empty)
bool is_Empty(list *list){
    return (list->last == -1);
}

//判断是否已满(is_Full)
bool is_Full(list *list){
    return (list->last >= list->cap - 1);
}

//将数据从小到大插入顺序表inser_List
void inser_List(list *list, int data){
    if(is_Full(list))
        return;
    
    if (is_Empty(list))
    {
        list->data[++list->last] = data;
        return;
    }
    
    // 1. 找到第一个比 data 大的位置 i
    int i;
    for (i = 0; i <= list->last; i++)
    {
        if (data < list->data[i])
            break;
    }
    
    // 2. 把 i 到 last 的元素整体后移一位（必须从后往前移）
    int j;
    for (j = list->last; j >= i; j--)
    {
        list->data[j+1] = list->data[j];
    }
    
    // 3. 插入 data，表长加 1
    list->data[i] = data;
    list->last++;
}

//删除顺序表中的指定数据del_List
bool del_List(list *list, int data){
    if(is_Empty(list))
        return false;
    
    int i, j;
    for ( i = 0; i <= list->last; i++)
    {
        if (list->data[i] == data)
        {
            for(j = i; j < list->last; j++){
                list->data[j] = list->data[j+1];
            }
            list->last--;
            return true;
        }    
    }
    
    return false;
}


//展示顺序表show_List
void show_List(list *list){
    if(is_Empty(list))
        return;
    
    int i;
    for(i = 0; i <= list->last; i++){
        printf("%d\t", list->data[i]);
    }
    return;
}

int main(int argc, char const *argv[])
{
    //初始化一条顺序表
    int cap = 10;
    list *list = init(cap);
    if (list == NULL)
    {
        printf("顺序表初始化失败\n");
        return 1;
    }

    //从键盘接收数字输入  
    int n;
    while (1)
    {
        if (scanf("%d", &n) == 1)
        {
            if(n == 0)
                break;
            if(n < 0){
                //如果输入负数删掉其绝对值的数
                if(is_Empty(list)){
                    printf("顺序表为空，无法删除\n");
                    continue;
                }
                del_List(list, abs(n));
            }else{
                //将键盘中收到的数字从小到大插入顺序表
                if(is_Full(list)){
                    printf("顺序表已满，无法插入\n");
                    continue;
                }
                inser_List(list, n);
            }
            show_List(list);
            printf("\n");
        }else{
            while (getchar() != '\n');
            break;
        }
    }
    return 0;
}
