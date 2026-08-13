// 3、（系统时间、标准IO、标准IO缓冲区处理）
// 编写一个程序，使之每隔1秒保存一次当前系统的时间和行号，保存到一个普通文件 time.txt中。要求在按 ctrl+c 终止该程序后，文件 time.txt 能正常保存到所有的信息，并且在重启程序能继续保存时间，并且保持行号连续，如下所示。

// gec@ubuntu:~$ ./a.out
//  1, Sat Jan  5 02:24:02 2020
//  2, Sat Jan  5 02:24:03 2020
//  3, Sat Jan  5 02:24:04 2020
// ^C
// gec@ubuntu:~$ ./a.out
//  4, Sat Jan  5 02:26:21 2020
//  5, Sat Jan  5 02:26:22 2020


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
int N; // 当前已有行数

// 读取line文件，得到当次行号
FILE *fp1 = fopen("line.bin", "r+"); // r w a r+ w+ a+
if(fp1 == NULL)
{
    N = 0;
    fp1 = fopen("line.bin", "w");
}
else
    fread(&N, sizeof(int), 1, fp1);


// 写入时间到 time.txt
FILE *fp2 = fopen("time.txt", "a"); // 追加模式打开

// char buf[50];  // 1, 年月日
while(1)
{
    // 获得当前年月日
    time_t t = time(NULL);
    char *str = ctime(&t);

    // printf(           "%d, %s", ++N, str);  // --> 终端
    // sprintf(buf,      "%d, %s", ++N, str);  // --> buf内存（容量）
    // snprintf(buf, 50, "%d, %s", ++N, str);  // --> buf内存（容量）
    fprintf(fp2,      "%d, %s", ++N, str);  // --> 文件

    // 更新行号（覆盖原有的行号）
    fseek(fp1, 0, SEEK_SET);
    fwrite(&N, sizeof(int), 1, fp1);

    fflush(fp1);
    fflush(fp2);

    sleep(1);
}

return 0;
}