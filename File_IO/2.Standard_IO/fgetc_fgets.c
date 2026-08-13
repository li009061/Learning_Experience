#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if(argc != 3){
        perror("open <源文件> <目标文件>");
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

    char s[100];
    while (1)
    {
        bzero(s, 100);
        if(fgets(s, 100, pf1) == NULL){
            if(feof(pf1))
                break;
            if(ferror(pf1)){
                perror("文件读取失败");
                break;
            }
        }

        fputs(s, pf2);
    }
    


    // int n;
    // while(1){
    //     n = fgetc(pf1);
    //     if(n == EOF){
    //         if(feof(pf1))
    //             break;
    //         if(ferror(pf1)){
    //             perror("fgetc faile");
    //             break;
    //         } 
    //     }

    //     fputc(n , pf2);
    // }

    return 0;
}
