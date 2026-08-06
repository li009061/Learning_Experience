// 构建一个顺序存储的循环队列，当0;输入数字时，将数字入队，当用户输入字母时，将队头元素出队。
// 每次操作队列之后，将队列中的元素显示出来。

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//定义顺序循环队列的结构体
typedef struct cycle_queue{
    int size;
    int *data;
    int front;  //头
    int rear ;  //尾
}queue;

//初始化循环队列init
queue * init(int size){
    queue *list = malloc(sizeof(queue));
    if(list != NULL){
        list->size = size;
        list->data = malloc(sizeof(int) * size);
        list->front = 0;
        list->rear = 0;
    }
    return list;
}

//销毁循环队列destroy
void destroy(queue *list){
    free(list->data);
    free(list);
}

//判断循环队列是否为空isEmpty
int isEmpty(queue *list){
    return list->front == list->rear;
}

//判断循环队列是否已满isFull
int isFull(queue *list){
    return (list->rear + 1) % list->size == list->front;
}

//显示循环队列show
void show(queue *list){
    int i = list->front;
    while(i != list->rear){
        printf("%d ", list->data[i]);
        i = (i + 1) % list->size;
    }
    printf("\n");
}

//入队enqueue
void enqueue(queue *list, int data){
    if(isFull(list)){
        printf("循环队列已满\n");
        return;
    }
    list->data[list->rear] = data;
    list->rear = (list->rear + 1) % list->size;
}

//出队dequeue
void dequeue(queue *list){
    if(isEmpty(list)){
        printf("循环队列已空\n");
        return;
    }
    list->front = (list->front + 1) % list->size;
}


int main(int argc, char const *argv[])
{
    //初始化循环队列结构体
    queue *list = init(10);
    if(list == NULL){
        perror("循环队列初始化失败\n");
        return 1;
    }else
        printf("循环队列初始化成功\n");

    //从键盘中读取数字or字母
    char n;
    printf("请输入数字（入队）或字母（出队），其他字符退出：\n");
    while(scanf("%c", &n) == 1){
        if(n == 0){
            break;
        }
        if (n == '\n' || n == ' ' || n == '\t') {
            continue;
        }
        if(n > '0' && n <= '9'){
            if(isFull(list)){
                printf("循环队列已满\n");
            }else{
                enqueue(list, n - '0');
            }
            show(list);
        }else if((n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z')){
            if(isEmpty(list)){
                printf("循环队列已空\n");
            }else{
                dequeue(list);
            }
            show(list);
        }else{
            break;
        }
    }
    destroy(list);
    return 0;
}


