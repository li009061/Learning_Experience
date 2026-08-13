#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    // 1. 判定参数合法性
    if(argc != 3)
    {
        printf("Usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    // 2. 打开源文件、目标文件
    FILE *fp1 = fopen(argv[1], "r");
    if(fp1 == NULL)
    {
        perror("打开源文件失败");
        return 1;
    }

    FILE *fp2 = fopen(argv[2], "w");
    if(fp2 == NULL)
    {
        fprintf(stderr, "打开目标文件【%s】失败:%s\n", argv[2], strerror(errno));
        return 1;
    }

    // 3. 读写文件
    char buffer[1024];
    while(1)
    {
        // 文本，人类可读     // 文本/二进制:mp3/mp4/rar/.gz/.jpg/.elf.....
        // fgetc/fgets/fscanf    fread
        // fputc/fputs/fprintf   fwrite

        // 当fgets返回NULL的时候，代表发生了错误，或者遇到了文件结尾
        // if(fgets(buffer, sizeof(buffer), fp1) == NULL)
        // {
        //     if(feof(fp1)) // 遇到了文件结尾
        //         break;
            
        //     if(ferror(fp1)) // 发生了错误
        //     {
        //         perror("读取源文件失败");
        //         break;
        //     }
        // }

        // fputs(buffer, fp2);

        // long a = lseek(fd, 0, SEEK_CUR); // 既调整位置、也获取位置
        // fseek();
        // 获取当前文件指针的位置
        long a = ftell(fp1);
            //放在哪里/读多少字节/读了多少块内存/从哪里读
        int n = fread(buffer, 20, 5, fp1); // n <= 5
                                           // 100: n=5
                                           // 99:  n=4
                                           // 95:  n=4
                                           // 88:  n=3
                                           // 21:  n=1
                                           // 18:  n=0
                                           //  2:  n=0
        if(n == 5)
            fwrite(buffer, 20, 5, fp2);
        else
        {
            // 发生了错误，或者遇到了文件结尾
            if(feof(fp1))
            {
                // 遇到了文件结尾
                long b = ftell(fp1);
                fwrite(buffer, b-a, 1 ,fp2);
                break;
            }
            if(ferror(fp1))
            {
                fprintf(stderr, "读取源文件失败");
                break;
            }
        }
    }

    // 4. 释放资源
    fclose(fp1);
    fclose(fp2);

    return 0;
}
