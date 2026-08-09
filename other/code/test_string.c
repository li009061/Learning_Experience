#include <stdio.h>
#include "string_utils.h"

int main() {
    // 1. 测试 str_concat
    char s1[100] = "Hello";
    char s2[] = " World";
    str_concat(s1, s2);
    printf("str_concat: \"%s\"\n", s1);

    // 2. 测试 str_remove_spaces
    char s3[] = "  Hello   World  ! ";
    str_remove_spaces(s3);
    printf("str_remove_spaces: \"%s\"\n", s3);

    // 3. 测试 str_remove_duplicates（双指针版）
    char s4[] = "abracadabra";
    str_remove_duplicates(s4);
    printf("str_remove_duplicates: \"%s\"\n", s4);

    // 4. 测试 str_remove_duplicates_other（标记数组版）
    char s5[] = "hello";
    str_remove_duplicates_other(s5);
    printf("str_remove_duplicates_other: \"%s\"\n", s5);

    // 5. 测试 str_length
    const char *s6 = "test123";
    int len = str_length(s6);
    printf("str_length(\"%s\") = %d\n", s6, len);

    return 0;
}