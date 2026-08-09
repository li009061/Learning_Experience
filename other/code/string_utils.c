#include <stdio.h>
#include <string.h>
#include "string_utils.h"

char* str_concat(char *dest, const char *src){
    char *p = dest;
    while (*p != '\0')
    {
        p++;
    }
    while (*src != '\0')
    {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';

    return dest;
}

// 去除字符串中所有空格
char* str_remove_spaces(char *str){
    char *p = str;
    char *q = str;

    while (*p != '\0')
    {
        if (*p != ' ')
        {
            *q = *p;
            q++;
        }
        p++;
    }
    *q = '\0';

    return str;
}

// 去除字符串中重复字符
char* str_remove_duplicates(char *str) {
    if (str == NULL) 
    return NULL;
    int len = strlen(str);
    if (len <= 1) 
    return str;
    int write = 1;
    for (int read = 1; read < len; read++) {
        int is_dup = 0;
        for (int check = 0; check < write; check++) {
            if (str[read] == str[check]) {
                is_dup = 1;
                break;
            }
        }
        if (!is_dup) {
            str[write] = str[read];
            write++;
        }
    }
    str[write] = '\0';
    return str;
}

char* str_remove_duplicates_other(char *str) {
    if (str == NULL) return NULL;

    int seen[256] = {0};
    int write = 0;

    for (int read = 0; str[read] != '\0'; read++) {
        unsigned char c = (unsigned char)str[read];
        if (!seen[c]) {
            seen[c] = 1;
            str[write++] = str[read];
        }
    }
    str[write] = '\0';
    return str;
}

// 计算字符串长度
int str_length(const char *s){
    const char *p = s;
    while (*p != '\0')
    {
        p++;
    }
return p - s;
}
