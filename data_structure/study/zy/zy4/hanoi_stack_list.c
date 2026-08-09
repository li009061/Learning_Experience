#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int datatype;

typedef struct stack
{
	datatype data;
	struct stack *next;
}stack;

stack *s1, *s2, *s3;

// 判断栈是否为空，若栈仅有头结点（即头结点的next为NULL）则返回true
bool is_empty(stack *s)
{
	return s->next == NULL;
}

// 获取栈的栈底节点（即链表最后一个节点）
stack *get_buttom(stack *s)
{
	while(s->next != NULL)
		s = s->next;

	return s;
}

// 向栈中压入一个新元素，同时更新栈底节点中记录的元素数量
void push(stack **p2top, int num)
{
	stack *new_node = (stack *)malloc(sizeof(stack));
	new_node -> data = num;
	new_node -> next = *p2top;

	// 将新节点设置为栈的新栈顶（因为是头插法，所以直接把栈顶指针指向新节点即可）
	*p2top = new_node;

	stack *p = get_buttom(*p2top);
	p->data++; 	
}

// 从栈中弹出栈顶元素，将弹出的值存入loc指向的位置，同时更新栈底记录的元素数量；栈为空时返回false
bool pop(stack **p2top, datatype *loc)
{
	if(is_empty(*p2top))
		return false;

	stack *s = *p2top;
	*loc = (*p2top) -> data;	
	*p2top = (*p2top) -> next;
	free(s);

	stack *p = get_buttom(*p2top);
	p->data--;

	return true;
}

// 以表格形式显示三个栈（三根柱子）的当前状态，用于汉诺塔搬运过程的可视化
void show(stack *s1, stack *s2, stack *s3)
{
	int maxlen, len, len1, len2, len3;

	len1 = get_buttom(s1)->data;
	len2 = get_buttom(s2)->data;
	len3 = get_buttom(s3)->data;

	maxlen = len1 > len2 ? len1 : len2;
	maxlen = maxlen > len3 ? maxlen : len3;
	len = maxlen;
	
	int i;
	for(i=0; i<maxlen; i++)
	{
		if(s1->next != NULL && len <= len1)
		{
			printf("%d", s1->data);
			s1 = s1->next;
		}
		printf("\t");
		
		if(s2->next != NULL && len <= len2)
		{
			printf("%d", s2->data);
			s2 = s2->next;
		}
		printf("\t");
		
		if(s3->next != NULL && len <= len3)
		{
			printf("%d", s3->data);
			s3 = s3->next;
		}
		printf("\n");		

		len--;
	}
	printf("s1\ts2\ts3\n-----------------\n");
}

// 使用递归方式反转一个数组
void revert(int a[], int len)
{
	if(len <= 1)
		return;

	// 递归处理去掉首尾元素后的子数组
	revert(a+1, len-2);

	// 交换首尾元素
	int tmp;
	tmp = a[0];
	a[0] = a[len-1];
	a[len-1] = tmp;
}

// 使用递归算法求解汉诺塔问题：将n个盘子从源柱ps1借助辅助柱ps3移动到目标柱ps2
void towers_of_hanoi(int n, stack **ps1, stack **ps2, stack **ps3)
{
	// tmp变量用于临时存放从栈中弹出的盘子数据，以便后续压入目标栈
	int tmp;

	// 递归终止条件：没有盘子需要移动时直接返回
	if(n <= 0)
		return;

	// 第一步：将n-1个盘子从源柱ps1借助目标柱ps2移动到辅助柱ps3上
	// 这样做是为了先把最大的盘子（第n个）露出来
	towers_of_hanoi(n-1, ps1, ps3, ps2);

	// 等待用户按键，然后显示当前三根柱子的状态
	// 将最底层（最大的那个）盘子从源柱ps1弹出，压入目标柱ps2
	getchar();
	show(s1, s2, s3);
	pop(ps1, &tmp);
	push(ps2, tmp);

	// 第二步：将之前放在辅助柱ps3上的n-1个盘子借助源柱ps1移动到目标柱ps2上
	// 这样就完成了将n个盘子从ps1移动到ps2的全过程
	towers_of_hanoi(n-1, ps3, ps2, ps1);
}

// 初始化一个空栈（创建头结点，data置为0表示栈中元素数量，next置为NULL）
void init(stack **ps)
{
	*ps = (stack *)malloc(sizeof(stack));
	(*ps)->data = 0;
	(*ps)->next = NULL;
}

// 主函数：读取汉诺塔盘子数量，初始化三根柱子的栈，将所有盘子按顺序放入s1，然后将汉诺塔从s1搬运至s2，最后打印结果
int main(void)
{
	printf("how many hanois ? ");
	int hanois;
	scanf("%d", &hanois);

	// 初始化了三个空栈
	init(&s1);
	init(&s2);
	init(&s3);

	// 将一些数据，放入s1里面
	int i;
	for(i=0; i<hanois; i++)
		push(&s1, hanois-i);

	// 将s1中的"汉诺塔"搬移到s2
	towers_of_hanoi(hanois, &s1, &s2, &s3);

	// 打印出最后的结果
	show(s1, s2, s3);

	return 0;
}
