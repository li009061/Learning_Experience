// 构建一个链式队列，当用户输入数字时，将数字入队，当用户输入字母时，
// 将队头元素出队。每次操作队列之后，将队列中的元素显示出来。
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//定义顺序链式队列的结构体
typedef struct link_queue{
    int data;
    struct link_queue *next;
}queue;

//管理结构体
typedef struct link_queue_manager{
    queue *fpont;  //队头指针
    queue *rear;   //队尾指针
    int size;
}link_queue;

//初始化链式队列init
link_queue * init(int size){
    link_queue *list = malloc(sizeof(link_queue));
    if(list != NULL){
        list->fpont = NULL;
        list->rear = NULL;
        list->size = 0;
    }
    return list;
}

//添加新节点addNode
queue *addNode(int data){
    queue *node = malloc(sizeof(queue));
    if(node != NULL){
        node->data = data;
        node->next = NULL;
    }
    return node;
}

//销毁链式队列destroy
void destroy(link_queue *list){
    queue *p = list->fpont;
    while(p != NULL){
        queue *tmp = p;
        p = p->next;
        free(tmp);
    }
    free(list);
}

//判断链式队列是否为空isEmpty
bool isEmpty(link_queue *list){
    return list->fpont == NULL;
}

//显示链式队列show
void show(link_queue *list){
    queue *p = list->fpont;
    while(p != NULL){
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

//入队enqueue
void enqueue(link_queue *list, int data){
    queue *node = addNode(data);
    if(node != NULL){
        if(isEmpty(list)){
            list->fpont = node;
            list->rear = node;
        }else{
            list->rear->next = node;
            list->rear = node;
        }
        list->size++;
    }
}

//出队dequeue
void dequeue(link_queue *list){
    if(isEmpty(list)){
        printf("链式队列已空\n");
        return;
    }
    queue *node = list->fpont;
    list->fpont = node->next;
    free(node);
    list->size--;
}


int main(int argc, char const *argv[])
{
    //初始化链式队列结构体
    link_queue *list = init(10);
    if(list == NULL){
        perror("链式队列初始化失败\n");
        return 1;
    }else
        printf("链式队列初始化成功\n");

    //从键盘中读取数字or字母
    char n;
    printf("请输入数字（入队）或字母（出队），其他字符退出：\n");
    while(scanf("%c", &n) == 1){
        if (n == '\n' || n == ' ' || n == '\t') {
            continue;
        }
        if(n >= '0' && n <= '9'){
            enqueue(list, n - '0');
            show(list);
        }else if((n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z')){
            if(isEmpty(list)){
                printf("链式队列已空\n");
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