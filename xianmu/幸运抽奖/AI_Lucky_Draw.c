#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

// 从 /dev/urandom 读取真随机数（基于硬件熵，不可预测）
unsigned int true_random(void) {
    unsigned int r = 0;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0) {
        read(fd, &r, sizeof(r));
        close(fd);
    }
    // 兜底：万一 urandom 打不开，用 time + getpid 混一个伪随机
    if (r == 0) {
        r = (unsigned int)(time(NULL) ^ getpid());
    }
    return r;
}

// 滚动演出：在候选区里随机滚动 total_steps 次，最后一步停在 stop 上
// pool_start: 候选区起点下标；stop: 最终停住的名字下标（绝对下标）
void roll(char **names, int pool_start, int count, int stop,
          int total_steps, int initial_delay, int final_delay) {
    for (int i = 0; i < total_steps; i++) {
        // 延迟从 final_delay 线性递增到 initial_delay，实现"从快到慢"
        int delay = final_delay + (i * (initial_delay - final_delay) / total_steps);

        // 最后一步停在 winner，其余在候选区随机滚动
        int idx = (i == total_steps - 1) ? stop
                  : pool_start + (true_random() % (count - pool_start));

        printf("\r%s\033[K", names[idx]);   // \033[K 清除行尾残留
        fflush(stdout);
        usleep(delay);
    }
}

int main() {
    FILE *fp = fopen("英雄名单.txt", "r");
    if (!fp) {
        perror("打开文件失败");
        return 1;
    }

    char **names = NULL;   // 指针数组
    int count = 0;
    int capacity = 0;
    char line[128];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';  // 去掉换行

        // 动态扩容
        if (count >= capacity) {
            capacity = (capacity == 0) ? 4 : capacity * 2;
            char **tmp = (char **)realloc(names, capacity * sizeof(char *));
            if (!tmp) {
                perror("内存分配失败");
                fclose(fp);
                free(names);
                return 1;
            }
            names = tmp;
        }

        // 为每个名字分配内存并拷贝
        names[count] = (char *)malloc(strlen(line) + 1);
        if (!names[count]) {
            perror("内存分配失败");
            fclose(fp);
            for (int i = 0; i < count; i++)
                free(names[i]);
            free(names);
            return 1;
        }
        strcpy(names[count], line);
        count++;
    }
    fclose(fp);

    // 空名单保护
    if (count == 0) {
        fprintf(stderr, "名单为空，无法抽奖\n");
        free(names);
        return 1;
    }

    // Fisher-Yates 洗牌：打乱顺序，之后按顺序取即不重复
    for (int i = count - 1; i > 0; i--) {
        int j = true_random() % (i + 1);
        char *tmp = names[i];
        names[i] = names[j];
        names[j] = tmp;
    }

    // 滚动参数
    int total_steps = 40;          // 总滚动次数
    int initial_delay = 150000;    // 最终延迟（微秒）= 150ms，最慢时能看清
    int final_delay = 5000;        // 初始延迟 = 5ms，飞快闪过

    printf("\n1. 一次抽取 N 个幸运儿（逐个滚动揭晓）\n");
    printf("2. 逐次抽取（每次 1 个，不重复）\n");
    printf("请选择模式: ");

    int mode;
    if (scanf("%d", &mode) != 1) {
        fprintf(stderr, "输入无效\n");
        for (int i = 0; i < count; i++) free(names[i]);
        free(names);
        return 1;
    }
    getchar();   // 吃掉残留的换行

    if (mode == 1) {
        int n;
        printf("请输入抽取人数: ");
        if (scanf("%d", &n) != 1) {
            fprintf(stderr, "输入无效\n");
            for (int i = 0; i < count; i++) free(names[i]);
            free(names);
            return 1;
        }
        getchar();

        if (n < 1 || n > count) {
            fprintf(stderr, "人数必须在 1~%d 之间\n", count);
            for (int i = 0; i < count; i++) free(names[i]);
            free(names);
            return 1;
        }

        printf("\n按 Enter 开始抽奖...\n");
        getchar();

        printf("\033[?25l");   // 隐藏光标

        for (int i = 0; i < n; i++) {
            // 逐个滚动揭晓：每次在 [i, count) 里滚动，最后停在 i
            roll(names, i, count, i, total_steps, initial_delay, final_delay);

            printf("\033[?25h");   // 恢复光标
            printf("\n🏆 第 %d 位幸运儿: %s\n", i + 1, names[i]);

            if (i < n - 1) {
                printf("按 Enter 揭晓下一位...");
                getchar();
                printf("\033[?25l");   // 再次隐藏光标
            }
        }

        printf("\n\n🎉 恭喜以上 %d 位幸运儿！\n", n);
        printf("\033[?25h");   // 确保光标恢复
    }
    else if (mode == 2) {
        int done = 0;   // 已抽取人数，也是下一位幸运儿的下标
        printf("\n按 Enter 抽取第 1 位幸运儿（输入 q + Enter 结束）...\n");

        while (done < count) {
            int c = getchar();
            if (c == EOF)
                break;
            if (c == 'q' || c == 'Q') {
                printf("已结束，共抽取 %d 位\n", done);
                break;
            }
            if (c != '\n')
                continue;

            printf("\033[?25l");
            fflush(stdout);

            // 候选区 [done, count)，最后停在 done
            roll(names, done, count, done, total_steps / 2, initial_delay, final_delay);

            printf("\033[?25h");
            printf("\n🏆 第 %d 位幸运儿: %s\n", done + 1, names[done]);
            done++;

            if (done < count)
                printf("按 Enter 抽取下一位（输入 q + Enter 结束）...\n");
            else
                printf("\n名单已全部抽完！\n");
        }
    }
    else {
        fprintf(stderr, "无效的模式\n");
    }

    // 释放内存
    for (int i = 0; i < count; i++)
        free(names[i]);
    free(names);
    return 0;
}