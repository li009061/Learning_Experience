// 汉诺塔图形动画版（"游戏"体验版）
// 用三个数组模拟三根柱子，每走一步就清屏重画一次，配合 usleep 产生动画效果
// 用法：./hanoi_game 然后输入盘子数 n（1 ~ 8，超过 8 步数太多动画会很长）
// 编译：gcc -o hanoi_game hanoi_game.c
// 运行环境需要支持 ANSI 转义（终端里直接跑即可，n=3 就能看到完整过程）

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_N 20

int stack[3][MAX_N];   // stack[t][i]：柱子 t 从下往上第 i 个盘子（数值 = 盘子编号，1 最小）
int top[3];            // 每根柱子上当前有几个盘子
int n;                 // 盘子总数
int step;              // 当前步数

// 初始化：n 个盘子全部叠在 0 号柱（底部编号最大，顶部编号最小）
void init(void)
{
    for (int i = 0; i < n; i++)
        stack[0][i] = n - i;   // 底部 n，顶部 1
    top[0] = n;
    top[1] = top[2] = 0;
    step = 0;
}

// 画一帧画面：从最顶层开始逐行画三根柱子
void draw(void)
{
    printf("\033[2J\033[H");   // ANSI 转义：清屏 + 光标回左上角
    printf("汉诺塔 —— 第 %d 步（共 %d 步）\n\n", step, (1 << n) - 1);

    int w = 2 * n + 1;         // 每根柱子占用的字符宽度（最大盘子的宽度 + 两侧余量）

    for (int row = n - 1; row >= 0; row--) {        // 从上往下逐行画
        for (int t = 0; t < 3; t++) {               // 三根柱子
            if (row < top[t]) {                     // 这一行有盘子
                int d = stack[t][row];
                int len = 2 * d - 1;                // 盘子越宽表示编号越大
                int space = (w - len) / 2;          // 居中
                for (int i = 0; i < space; i++) putchar(' ');
                for (int i = 0; i < len; i++) putchar('=');
                for (int i = 0; i < space; i++) putchar(' ');
            } else {                                // 空位置画柱身
                for (int i = 0; i < w / 2; i++) putchar(' ');
                putchar('|');
                for (int i = 0; i < w / 2; i++) putchar(' ');
            }
            putchar(' ');                           // 柱子之间的间隔
        }
        putchar('\n');
    }

    for (int t = 0; t < 3; t++) {                   // 柱底
        for (int i = 0; i < w; i++) putchar('-');
        putchar(' ');
    }
    printf("\n  A");
    for (int i = 0; i < w + 1; i++) putchar(' ');
    printf("B");
    for (int i = 0; i < w + 1; i++) putchar(' ');
    printf("C\n");
}

// 执行一次移动：从 from 柱顶部取一个盘子，放到 to 柱顶部
void move_disk(int from, int to)
{
    int d = stack[from][top[from] - 1];   // 取出顶部盘子
    top[from]--;
    stack[to][top[to]] = d;
    top[to]++;
    step++;
    draw();               // 重画
    usleep(300000);       // 停 0.3 秒，动画效果
}

// 递归：把 k 个盘子从 from 借助 aux 移到 to
void hanoi(int k, int from, int aux, int to)
{
    if (k == 1) { move_disk(from, to); return; }
    hanoi(k - 1, from, to, aux);
    move_disk(from, to);
    hanoi(k - 1, aux, from, to);
}

int main(void)
{
    printf("请输入盘子个数 n（1 ~ 8）：");
    scanf("%d", &n);
    if (n < 1 || n > 8) {
        printf("n 超出范围\n");
        return 1;
    }

    init();
    draw();               // 画初始状态（第 0 步）
    usleep(800000);
    hanoi(n, 0, 1, 2);    // 0=A, 1=B, 2=C

    printf("\n完成！共移动 %d 步（2^%d - 1 = %d）\n", step, n, (1 << n) - 1);
    return 0;
}
