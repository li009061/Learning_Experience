#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define EPRINT(func, fmt, args...){ \
    time_t t = time(NULL);\
    printf("%s", strtok(ctime(&t), "\n")); \
    printf("[%d @ %s] :", __LINE__, __FILE__);\
    printf("%s, ", #func);\
    printf(fmt, ##args);\
}

int test_failed(void)
{
    return 1;
}

#define WHEN(func, act) { \
    int retcode = func; \
    if(retcode != 0) \
    { \
        time_t t = time(NULL); \
        printf("%s ", strtok(ctime(&t), "\n")); \
        printf("[%d @ %s]: ", __LINE__, __FILE__); \
        printf("%s, return -1\n", #func); \
        act; \
    } \
}

int main(int argc, char const *argv[])
{
    EPRINT(test(10), "->%s: %d\n", "hello", 2345);
    WHEN(test_failed(), return -1);
    
    return 0;
}
