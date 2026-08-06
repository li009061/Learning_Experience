#include <math.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>


// 数据规模: 100万
#define SCALE  1000*1000 // 字符串总数
#define MAXLEN 20        // 字符串最大长度

// 查找次数统计
static int count;

char *random_string()
{
    int len = rand()%(MAXLEN);
    len = ((len<2)? 2 : len); // len: 2~19

    char *s = calloc(1, len);
    char letter[] = {'a', 'A'};

    for(int i=0; i<len-1; i++) // i: 0~[1~18]
        s[i] = letter[rand()%2]+rand()%26; //每个字符都是随机选择的大写或小写字母。

    return s;
}

void store(char data[][MAXLEN], const char *file)
{
    FILE *fp = fopen(file, "w");
    for(int i=0; i<SCALE; i++)
        fprintf(fp, "%06d.%s\n", i, data[i]); // 数据规模：100万（6位数）

    fclose(fp);
}

void create_index(char data[][MAXLEN], int **index)
{
    // 统计各个首字符出现的频次
    int n[52]={0}; // ['a', 'b', ... 'z', 'A', 'B', ... 'Z']
    for (int k = 0; k < SCALE; k++)
    {
        // 小写字母[00~25]，大写字母[26~51]
        int pos = ((data[k][0] >= 'a') ? (data[k][0]-'a') : (data[k][0]-'A'+26));
        n[pos]++;
    }

    // 给index分配内存
    // 每个字母分配一段存储以该字母为首的字符串所在的行号的内存
    // 第一个位置存储总行数，因此所需分配的内存单元数是1+n[i]。
    // 例如:
    // index[2] --> [ 242     3     22    213 ... ... 42513 46698]
    //   'c'    -->  总行数  第3行 第22行     ... ...
    for(int i=0; i<52; i++)
        index[i] = calloc(1+n[i], sizeof(int));

    // 记录每个字母出现的行号
    for(int i=0; i<SCALE; i++)
    {
        int pos = ((data[i][0] >= 'a') ? (data[i][0]-'a') : (data[i][0]-'A'+26));

        int k = ++index[pos][0];
        index[pos][k] = i;
    }
}

int main(int argc, char const *argv[])
{
    // 1. 产生随机字符串数据集
    //    假设每个字符串长度不超过MAXLEN个字符
    char (*data)[MAXLEN] = calloc(SCALE, MAXLEN);

    srand(time(NULL));
    for(int i=0; i<SCALE; i++)
    {
        char *s = random_string();
        strncpy(data[i], s, strlen(s));
        free(s);
    }
    store(data, "a.txt");

    // 2. 按首字母建立索引（分块）
    int **index = calloc(52, sizeof(int *));
    create_index(data, index);

    // 3. 利用索引，进行查找
    char str[32];
    printf("请输入你要查找的字符串:\n");
    while(1)
    {
        // 从键盘接收一个待查找的字符串并去掉回车符
        bzero(str, 32);
        fgets(str, 32, stdin);
        strtok(str, "\n");

        bool done = false;
        for(int i=1; i<SCALE; i++)
        {
            // 小写字母[00~25]，大写字母[26~51]
            int pos = ((str[0]>='a') ? (str[0]-'a') : (str[0]-'A'+26));

            count++;
            if(i<=index[pos][0] && strcmp(data[index[pos][i]], str) == 0)
            {
                printf("你要找的字符串在第%d行", index[pos][i]);
                done = true;
                break;
            }
            else if(i > index[pos][0])
                break;
        }

        if(!done)
            printf("没有你要的字符串");

        printf("【找了%d次】\n", count);
        count=0;
    }

    return 0;
}