// 1、（排序）
// 刚举行的万米长跑活动中，有 N 个人跑完了全程，所用的时间都不相同。
// 颁奖时为了增加趣味性，随机抽了一个数K，要奖励第 K 名一双跑鞋。
// 现在组委会提供给你 N 个人的姓名、成绩（用时，单位是秒）的比赛数据文件，请你编程快速输出第 K 名的姓名。

#include <stdio.h>

#define MAX 100   // 最多支持 100 名选手

//定义一个成员的结构体
typedef struct {
    char name[64];
    int time;
} runner;

//交换（结构体版：runner 有 name 和 time 两个字段，必须整体交换）
void swap_runner(runner *a, runner *b){
    runner tmp = *a;
    *a = *b;
    *b = tmp;
}

//快速排序：Lomuto 分区，按 time 升序排列（第 1 名 = 用时最少）
int partition(runner data[], int low, int high){
    int pivot = data[high].time;   // 基准：最后一个元素的成绩
    int i = low - 1;               // i：最后一个"成绩比 pivot 小"的下标

    for (int j = low; j < high; j++) {
        if (data[j].time < pivot) {          // 比基准小 → 换到左边区域
            i++;
            swap_runner(&data[i], &data[j]);
        }
    }
    swap_runner(&data[i + 1], &data[high]);  // 基准归位
    return i + 1;                            // 返回基准的最终下标
}

void quickSort(runner data[], int low, int high){
    if (low < high) {
        int p = partition(data, low, high);
        quickSort(data, low, p - 1);    // 排左边（都比基准小）
        quickSort(data, p + 1, high);   // 排右边（都比基准大）
    }
}

//二分查找：按成绩找人（成绩互异才能用）
//说明：找"第 K 名"其实用不到它（排序后按下标直接取），
//     它适合"给一个成绩，查这个人排第几"的场景，这里作为附加验证。
int binary_search(runner data[], int n, int target){
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (data[mid].time == target)      return mid;        // 找到了
        else if (data[mid].time < target)  low = mid + 1;     // 去右半边
        else                               high = mid - 1;    // 去左半边
    }
    return -1;   // 没找到
}

int main(void) {
    //打开文件并读取数据
    FILE *fp = fopen("record.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败\n");
        return 1;
    }

    runner data[MAX];
    int count = 0;
    // %63s：限制名字最多 63 个字符（防缓冲区溢出）；文件名是中文无空格，%s 一次读完
    while (count < MAX && fscanf(fp, "%63s %d", data[count].name, &data[count].time) == 2) {
        count++;
    }
    fclose(fp);
    printf("共读取 %d 名选手\n", count);

    //输入名次 K（范围 1 ~ count）
    int n;
    printf("请输入名次K(1~%d): ", count);
    //修复：原来 && 应为 ||，且名次范围应为 1~count（原来写 53 是错的）
    while (scanf("%d", &n) != 1 || n < 1 || n > count) {
        printf("输入无效，请重新输入名次K(1~%d): ", count);
        while (getchar() != '\n');   // 清空残留的非法输入
    }

    //快速排序：升序排完，第 K 名就是下标 K-1（下标从 0 开始）
    quickSort(data, 0, count - 1);

    printf("第%d名：%s，成绩 %d 秒\n", n, data[n - 1].name, data[n - 1].time);

    //附加验证：用二分查找确认"第 K 名的成绩"在有序数组中的下标 = K-1
    int idx = binary_search(data, count, data[n - 1].time);
    printf("（二分查找验证：成绩 %d 秒位于数组下标 %d，即第 %d 名）\n",
           data[n - 1].time, idx, idx + 1);

    return 0;
}
