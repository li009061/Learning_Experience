#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if(argc != 3){
    fprintf(stderr, "用法: %s <源文件> <目标文件>\n", argv[0]);
        return -1;
    }

    FILE *pf1 = fopen(argv[1], "r");
    if(pf1 == NULL){
        perror("文件打开失败");
        return -1;
    }

    FILE *pf2 = fopen(argv[2], "w");
    if(pf2 == NULL){
        perror("文件打开失败");
        return -1;
    }

    char s[20*5];
    int nread;
    long begin, end;
    while (1)
    {
        begin = ftell(pf1);

        bzero(s, 20*5);     //memset(s, 0, sizeof(s));
        nread = fread(s, 20, 5, pf1);
        if(nread < 5){
            if(feof(pf1)){
                end = ftell(pf1);
                fwrite(s, end-begin, 1, pf2);
                break;

            }
        }
        if (ferror(pf1))
        {
            perror("文件读取失败");
            return -1;
        }
        
        fwrite(s, 20, 5, pf2);
    }
    

    return 0;
}