#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//管理循环队列的结构体
struct cycle_Queue
{
    int *data;  //----存放的数据
    int room;   // ---容量
    int front;  //----队列的头的下标
    int rear;   //----队列的尾的下标
};

//初始化一个空的循环队列init_Queue
struct cycle_Queue * init_Queue(int room){
    struct cycle_Queue * queue = calloc(1,sizeof(struct cycle_Queue));
    if (queue == NULL)
        return NULL;
    
    queue->data = calloc(room+1,sizeof(int));
    if (queue->data == NULL)
    {
        free(queue);
        return NULL;
    }
    queue->room = room + 1;
    queue->front = queue->rear = 0;
    
    return queue;
}

//判断队列是否为空is_Empty
bool is_Empty(struct cycle_Queue *queue){
    return (queue->rear == queue->front);
}

//判断队列是否已满is_Full
bool is_Full(struct cycle_Queue *queue){
    return (((queue->rear)+1)%(queue->room) == queue->front);
}

//入队inQueue
bool inQueue(struct cycle_Queue *queue, int data){
    if(is_Full(queue))
        return false;
    
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % queue->room;
    return true;
}

//出队outQueue
bool outQueue(struct cycle_Queue *queue, int *a){
    if(is_Empty(queue))
        return false;

    *a = queue->data[queue->front];
    queue->front = (queue->front+1) % queue->room;
    return true;
}

//展示队列中的数据
void showQueue(struct cycle_Queue *queue){
    if(is_Empty(queue))
        return;

    for(int i = queue->front; i != queue->rear; i = (i+1)%queue->room){
        printf("%d\t", queue->data[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    //初始化一个空的循环队列
    struct cycle_Queue *queue = init_Queue(10);
    if (queue == NULL)
        perror("循环队列初始化失败");
    else
        printf("循环队列初始化成功!\n");    
    
    //入队，出队，展示队列
    int n;
    while(1){
        // 写入一些数据
        if(scanf("%d", &n) == 1){
            if (!inQueue(queue, n))
            {
                printf("队列空间已满，入队失败。\n");
                continue;
            }
        } else{
            //清空输入缓冲区
            while(getchar() != '\n');
            //出队
            int m;
            if (!outQueue(queue, &m)){
                printf("队列为空，出队失败。\n");
                continue;
            }
        }
        
        //展示队列
        showQueue(queue);
    }

    return 0;
}
