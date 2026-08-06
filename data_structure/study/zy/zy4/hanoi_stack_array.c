#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static int count = 1;

// 顺序栈管理结构体
typedef struct 
{
    int *data; // 顺序栈入口
    int size;  // 顺序栈总容量
    int top;   // 顺序栈栈顶元素下标
}seqStack;

seqStack *s1;
seqStack *s2;
seqStack *s3;

// 初始化空栈
seqStack *initStack(int size)
{
    seqStack *s = (seqStack *)malloc(sizeof(seqStack));
    if(s != NULL)
    {
        s->data = (int *)malloc(sizeof(int) * size);
        if(s->data == NULL)
        {
            free(s);
            return NULL;
        }
    }

    s->size = size;
    s->top  = -1;
    return s;
}

// 判断栈是否已满
bool isFull(seqStack *s)
{
    return s->top == s->size-1;
}

// 判断栈是否为空
bool isEmpty(seqStack *s)
{
    return s->top == -1;
}

// 入栈
bool push(seqStack *s, int data)
{
    if(isFull(s))
        return false;

    s->data[++s->top] = data;
    return true;
}

// 取栈顶元素
bool top(seqStack *s, int *pm)
{
    if(isEmpty(s))
        return false;

    *pm = s->data[s->top];
    return true;
}

// 出栈
bool pop(seqStack *s, int *pm)
{
    if(top(s, pm) == false)
        return false;

    s->top--;
    return true;
}


void show()
{
	int i, maxlen;

	int top1 = s1->top;
	int top2 = s2->top;
	int top3 = s3->top;

	maxlen = top1 > top2 ? top1 : top2;
	maxlen = (maxlen > top3 ? maxlen : top3) + 1;

	int a, b, c;
	a = b = c = maxlen;

	for(i=0; i<maxlen; i++)
	{
		if(top1 != -1 && a == top1+1)
			printf("%d", (s1->data)[top1--]);
		a--;
		printf("\t");
		
		if(top2 != -1 && b == top2+1)
			printf("%d", (s2->data)[top2--]);
		b--;
		printf("\t");
		
		if(top3 != -1 && c == top3+1)
			printf("%d", (s3->data)[top3--]);
		c--;
		printf("\n");
	}
	printf("s1\ts2\ts3\n-----------------\n\n");
	printf("count=%d\n", count++);
}

// 将hanios个汉诺塔，从ps1搬到ps2，借助于ps3
void hanoi(int size, seqStack *s1, seqStack *s2, seqStack *s3)
{
	if(size == 0)
		return;

	hanoi(size-1, s1, s3, s2);

	show();
	getchar();

	int tmp;
	pop(s1, &tmp);
	push(s2, tmp);

	hanoi(size-1, s3, s2, s1);
}


int main(void)
{
	s1 = initStack(10);
	s2 = initStack(10);
	s3 = initStack(10);

    // 假设起始状态，在第一个汉诺塔中含有5个圆饼
    for(int i=1; i<=5; i++)
        push(s1, i);

	hanoi(5, s1, s2, s3);
	show();

	return 0;
}
