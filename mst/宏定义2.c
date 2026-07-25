// 4.	如下代码，请设计宏定义STR(x)，将USART_RATE转换成字符串并打印出来
#include <stdio.h>

#define USART_RATE 115200
#define STR(x) STR_H(x)
#define STR_H(x) #x
//      STR(USART_RATE)_____

// #define USART_RATE 115200
// #define STR(x) #x


int main(int argc, char const *argv[])
{
    
    printf("usart rate = %s\n", STR(USART_RATE));
    //printf("usart rate = %s\n",STR(115200));
    return 0;
}

//总结:字符串化 # 不展开宏，需要套一层宏先展开再字符串化。
//标准 C 中，一层宏无法实现。# 运算符的特性是先字符串化、不展开参数。
// 必须使用两层宏：第一层展开 USART_RATE → 115200，第二层用 # 字符串化。
