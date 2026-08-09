#include <stdio.h>
#include "math_utils.h"

//返回三个值中的最大值
int max3(int a, int b, int c){
    int max = (a > b) ? a : b;
    max = (max >  c) ? max : c;
    return max;
}

// 循环实现 x 的 n 次方
float myPower_cycle(float x,int n){
{
    int i;
    float tmp = x;
    for(i=0; i<n-1; i++)
    {
        x *= tmp;
    }
    return x;
}
}

// 递归实现x 的 n 次方
float myPower_recursion(float x, int n){
        if (n == 0){ 
            return 1.0f;
        }
        if (n > 0 )
        {
            return x * myPower_recursion(x , n-1);
        }
        return 1.0f / myPower_recursion(x , -n);
}

// 迭代求第 n 项斐波那契数
int fibonacci_iter(int n){
        switch (n)
    {
    case 1:
        return 0;
    case 2:
        return 1;
    }
    return fibonacci_iter(n-1) + fibonacci_iter(n-2);
}

// 判断一个整数是否为素数
int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

//冰雹猜想到达1的步数
int hailstone_steps(unsigned int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

//三点求圆心
int get_circle_center(double x1, double y1, double x2, double y2, double x3, double y3,
                      double *cx, double *cy) {
    double A = 2 * (x2 - x1);
    double B = 2 * (y2 - y1);
    double C = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;

    double D = 2 * (x3 - x1);
    double E = 2 * (y3 - y1);
    double F = x3 * x3 + y3 * y3 - x1 * x1 - y1 * y1;

    double det = A * E - B * D;

    if (fabs(det) < 1e-12) {
        return 0;   // 三点共线，无解
    }

    *cx = (C * E - B * F) / det;
    *cy = (A * F - C * D) / det;
    return 1;
}