// 1.	给定字符串，去除重复字母后重新输出，
// 如输入s="abcaadc"，输出s="abcd"，其中 s 长度小于等于 10000
#include <stdio.h>

void strings_remove(char *s){
    char a[256] = {0};
    char s1[10000];
    int j = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if(a[s[i]] == 0){
            s1[j] = s[i];
            j++;
            a[s[i]] = 1;
        }
    }
    s1[j] = '\0';
    printf("%s\n", s1);
    printf("%d", j);
}

int main(int argc, char const *argv[])
{
    char s[10000];
    while (1)
    {
        if (scanf("%s", s) != 1) {
            printf("输入错误\n");
            while (getchar() != '\n');
            continue; 
        }
        break;
    }
    
    strings_remove(s);

    return 0;
}




// void string_Remove_duplicates(char *p){
//     char s[256] = {0};
//     char a[256];
//     int j = 0;
//     for (int i = 0; p[i] != '\0'; i++)
//     {
//         if (s[p[i]] == 0)
//         {
//             a[j] = p[i];
//             j++;
//             s[p[i]] = 1;
//         }
//     }
//     a[j] = '\0';
//     printf("%s\n", a);
//     printf("%d", j);
// }

// int main(int argc, char const *argv[])
// {
//     char s[] = {"abbchfceecdef"};
//     string_Remove_duplicates(s);
//     printf("\n");
//     return 0;
// }
