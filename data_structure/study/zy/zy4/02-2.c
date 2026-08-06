#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static int count = 1;

seqStack *s1;
seqStack *s2;
seqStack *s3;

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