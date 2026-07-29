// 37.	输入一组字符串数据 (该字符串为 0-9.a-f,A-F 组成)，需要转成 16 进制数据导出。例（“1234”）导出 (0x12,0x34)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char hex_s[50];
    fgets(hex_s, 50, stdin); // "ab"
    // strtok(hex_s, "\n");
    hex_s[strlen(hex_s)-1] = '\0';

    // 1. 判定输入是否合法: 偶数位、0-9A-F
    if(strlen(hex_s) % 2 != 0)
    {
        printf("必须输入偶数位\n");
        return 0;
    }

    int len = strlen(hex_s);

    for(int i=0; i<len; i++)
    {
        if( ('0'<=hex_s[i] && hex_s[i]<='9') ||
            ('a'<=hex_s[i] && hex_s[i]<='f') ||
            ('A'<=hex_s[i] && hex_s[i]<='F')  )
            continue;
        else
        {
            printf("输入不合法\n");
            return 0;
        }
    }

    // "CB1A8876A"
    // 0xCB 0x1A 0x88 ...
    // 2. 挨个字符转换整数
    // int hex[50];

    for(int i=0; i<len; i+=2)
    {
        char low, high;
        high = hex_s[i];
        low  = hex_s[i+1];

        int a = 0;
        switch(high)
        {
        case '0' ... '9':
            a = (high-'0') << 4;
            break;

        case 'A' ... 'F':
            a = (high-'A'+10 << 4);
            break;

        case 'a' ... 'f':
            a = (high-'a'+10 << 4);
            break;
        }

        switch(low)
        {
        case '0' ... '9':
            a |= (low-'0');
            break;

        case 'A' ... 'F':
            a |= (low-'A'+10);
            break;

        case 'a' ... 'f':
            a |= (low-'a'+10);
            break;
        }

        printf("0x%02X\n", (unsigned char)a); 
    }

    return 0;
}
