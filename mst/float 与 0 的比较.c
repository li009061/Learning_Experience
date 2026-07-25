//写出 float x 与 “零值” 比较的 if 语句
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

float MIN_FLOAT = 0.1e-44;

int main(void)
{
    float x1 = 0.1e-43;
    float x2 = 0.1e-45;
    if(x1 < MIN_FLOAT)
    printf("等于0\n");
    else printf("不等于0\n");
    if(x2 < MIN_FLOAT)
    printf("等于0\n");
    else printf("不等于0\n");
        float x = 0.0000001f;   // 一个很小的数
    
    // 标准写法：判断绝对值是否小于精度阈值
    if (fabsf(x) < FLT_EPSILON) {
        printf("x 等于零（在精度范围内）\n");
    } else {
        printf("x 不等于零\n");
    }
    
    // 或者自定义精度（更宽松）
    #define EPS 1e-6f
    if (fabsf(x) < EPS) {
        printf("x 约等于零\n");
    }
    
    return 0;
}

