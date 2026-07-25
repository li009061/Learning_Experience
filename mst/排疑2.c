// 11.	找出下面这段代码的运行错误：
#include <stdio.h>
void test1()
{
    char string[10];
    char* str = "0123456789";
    strcpy( string, str );  
}
// 方法一：增大目标数组
// char string[11];  // 至少 11 字节
// strcpy(string, "0123456789");

// 方法二：使用 strncpy 限制复制长度
// char string[10];
// strncpy(string, "0123456789", sizeof(string) - 1);
// string[sizeof(string) - 1] = '\0';  // 手动添加结束符

// 方法三：动态分配（如果需要）
// char *string = malloc(11);
// if (string) {
//     strcpy(string, "0123456789");
//     // 使用后 free
// }

void GetMemory( char* p )
{
    p = (char*) malloc( 100 );
}
void Test( void )
{
    char* str = NULL;
    GetMemory( str );
    strcpy( str, "hello world" );
    printf( str );
}

// 方法一：返回指针
// char * GetMemory( )
// {
//     return (char*) malloc( 100 );
// }
// void Test( void )
// {
//     char* str = NULL;
//     str = GetMemory();
//     strcpy( str, "hello world" );
//     printf( str );
// }

//方法二：二级指针
// void GetMemory(char** p) {
//     *p = (char*)malloc(100);
// }
// void Test() {
//     char* str = NULL;
//     GetMemory(&str);
//     strcpy(str, "hello world");
//     printf("%s\n", str);
//     free(str);
// }