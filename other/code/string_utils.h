#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// 字符串的连接
char* str_concat(char *dest, const char *src);

// 去除字符串中所有空格
char* str_remove_spaces(char *str);

// 去除字符串中重复字符
char* str_remove_duplicates(char *str);

// 去除字符串中重复字符(标记数组法)
char* str_remove_duplicates_other(char *str);

// 计算字符串长度
int str_length(const char *s);

#endif