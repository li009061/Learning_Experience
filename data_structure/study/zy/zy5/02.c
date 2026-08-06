// 2、（排序）我的版本：快速选择 QuickSelect
// 题目说"快速输出第 K 名"——最快的做法不是把数组全部排好序，
// 而是用快排的 partition 思想，只把第 K 名"归位"就停：
// 每轮分区后，第 K 名只可能在左半边或右半边，只递归那一边。
// 平均 O(n)（快排是 O(n log n)），这就是"快速"二字的最优解。

#include <stdio.h>

#define MAX 100   // 最多 100 名选手

typedef struct {
    char name[64];
    int  time;
} Player;

// 交换两个选手（整个结构体一起换）
static void swap(Player *a, Player *b)
{
    Player t = *a;
    *a = *b;
    *b = t;
}

// Lomuto 分区：以最后一个成绩为基准，升序
// 返回基准的最终下标：左边全比它小，右边全比它大
static int partition(Player *p, int low, int high)
{
    int pivot = p[high].time;
    int i = low - 1;               // 最后一个"比 pivot 小"的下标

    for (int j = low; j < high; j++) {
        if (p[j].time < pivot) {
            i++;
            swap(&p[i], &p[j]);
        }
    }
    swap(&p[i + 1], &p[high]);
    return i + 1;
}

// 快速选择：把第 k 名（0 基下标）归位，其余部分不管
// 与快排唯一区别：快排两边都递归，这里只递归包含 k 的那一边
static void quick_select(Player *p, int low, int high, int k)
{
    if (low >= high)               // 区间空或只剩一个，天然就位
        return;

    int m = partition(p, low, high);

    if (m == k)                    // 第 k 名正好归位，完成！
        return;
    if (k < m)
        quick_select(p, low, m - 1, k);     // k 在左半边 → 只排左边
    else
        quick_select(p, m + 1, high, k);    // k 在右半边 → 只排右边
}

int main(void)
{
    FILE *fp = fopen("record.txt", "r");
    if (fp == NULL) {
        perror("record.txt");
        return 1;
    }

    Player p[MAX];
    int n = 0;
    while (n < MAX && fscanf(fp, "%63s %d", p[n].name, &p[n].time) == 2)
        n++;
    fclose(fp);

    int k;
    printf("请输入名次(1~%d): ", n);
    while (scanf("%d", &k) != 1 || k < 1 || k > n) {
        while (getchar() != '\n');          // 清空非法输入
        printf("输入无效，请重新输入名次(1~%d): ", n);
    }

    // 第 k 名 = 成绩第 k 小的 = 升序下标 k-1
    quick_select(p, 0, n - 1, k - 1);

    printf("第%d名：%s，成绩 %d 秒\n", k, p[k - 1].name, p[k - 1].time);
    return 0;
}
