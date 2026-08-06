// 编程实现汉诺塔游戏（递归版）
// 目标：把 n 个盘子从 A 柱借助 B 柱移到 C 柱，打印每一步移动并统计步数
// 核心思想：递归 —— 先把 n-1 个盘子搬走，再动最底下的盘子，最后把 n-1 个搬回来
//   ① hanoi(n-1, A, C, B)：上面 n-1 个盘子 A -> B（借助 C）
//   ② A -> C：最底下的大盘子直接移动
//   ③ hanoi(n-1, B, A, C)：B 上的 n-1 个盘子 B -> C（借助 A）
// 递归出口：n == 1 时只剩一个盘子，直接移动
// 总步数 = 2^n - 1

#include <stdio.h>

long long step = 0;   // 步数统计，用 long long 防止 n 较大时溢出

// hanoi: 把 n 个盘子从 from 柱借助 aux 柱移到 to 柱
void hanoi(int n, char from, char aux, char to)
{
    if (n == 1) {                                     // 递归出口：只有一个盘子
        printf("第%lld步：%c -> %c\n", ++step, from, to);
        return;
    }
    hanoi(n - 1, from, to, aux);      // ① 上面的 n-1 个盘子先搬到辅助柱
    printf("第%lld步：%c -> %c\n", ++step, from, to); // ② 最底下的大盘子搬到目标柱
    hanoi(n - 1, aux, from, to);      // ③ 辅助柱上的 n-1 个盘子搬到目标柱
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

    hanoi(n, 'A', 'B', 'C');

    printf("--------------------------------\n");
    printf("总步数：%lld\n", step);
    printf("公式验证：2^%d - 1 = %lld\n", n, (1LL << n) - 1);
    return 0;
}
