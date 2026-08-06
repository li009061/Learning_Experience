// 编程实现汉诺塔游戏（用栈）
// 思路：递归版靠系统维护"函数调用栈"，这里自己用数组模拟一个栈来重现递归过程。
// 每个栈帧 = 一次 hanoi 调用的现场：{n, from, aux, to, flag}
//   flag 相当于"程序计数器"，标记这次调用执行到哪一步：
//     0 = 刚进入函数，还没执行第一段子任务
//     1 = 第一段完成，该执行第二段（移动 from -> to）
//     2 = 第二段完成，该执行第三段（第三段做完，函数返回）
// 递归体 hanoi(n, from, aux, to) 等价于依次做三件事：
//   ① hanoi(n-1, from, to, aux)   把上面 n-1 个盘子搬到辅助柱
//   ② from -> to                  移动最底下的大盘子
//   ③ hanoi(n-1, aux, from, to)   把 n-1 个盘子搬到目标柱
// 栈是后进先出，所以"先压后面要做的"，子任务才能先弹出执行。

#include <stdio.h>

typedef struct {
    int n;          // 本次调用要处理的盘子数
    char from;      // 起点柱
    char aux;       // 辅助柱
    char to;        // 终点柱
    int flag;       // 执行到第几步（0/1/2）
} Frame;

long long step = 0;   // 步数统计

// 用显式栈模拟递归，完成汉诺塔
void hanoi_stack(int n, char from, char aux, char to)
{
    Frame stack[3 * 64];   // 栈深最多约 3n 层，开 192 足够（n <= 20）
    int top = 0;           // 栈顶指针，top == 0 表示空栈

    stack[top++] = (Frame){n, from, aux, to, 0};   // 初始任务入栈

    while (top > 0) {
        Frame f = stack[--top];    // 弹出当前要执行的调用

        if (f.flag == 0) {                         // 刚进入函数
            if (f.n == 1) {                        // 递归出口：只剩一个盘子
                printf("第%lld步：%c -> %c\n", ++step, f.from, f.to);
            } else {
                // 先执行第一段 hanoi(n-1, from, to, aux)：
                // 把自己的"剩余现场"(flag=1)压回栈，再把子任务①压栈
                stack[top++] = (Frame){f.n, f.from, f.aux, f.to, 1};
                stack[top++] = (Frame){f.n - 1, f.from, f.to, f.aux, 0};
            }
        } else if (f.flag == 1) {                  // 第一段完成，执行第二段：移动
            printf("第%lld步：%c -> %c\n", ++step, f.from, f.to);
            // 把自己(flag=2)压回，再压第三段子任务 hanoi(n-1, aux, from, to)
            stack[top++] = (Frame){f.n, f.from, f.aux, f.to, 2};
            stack[top++] = (Frame){f.n - 1, f.aux, f.from, f.to, 0};
        }
        // flag == 2：第三段已执行完，函数返回，什么都不做
    }
}

int main(void)
{
    int n;
    printf("请输入盘子个数 n：");
    scanf("%d", &n);

    if (n < 1) {
        printf("n 必须 >= 1\n");
        return 1;
    }
    if (n > 20) {   // 防止 2^n 溢出 long long，也避免步骤刷屏
        printf("n 太大，最多支持 20\n");
        return 1;
    }

    hanoi_stack(n, 'A', 'B', 'C');

    printf("--------------------------------\n");
    printf("总步数：%lld\n", step);
    printf("公式验证：2^%d - 1 = %lld\n", n, (1LL << n) - 1);
    return 0;
}
