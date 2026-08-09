#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// 返回三个值中的最大值
int max3(int a, int b, int c);

// 循环实现 x 的 n 次方
float myPower_cycle(float x, int n);

// 递归实现x 的 n 次方
float myPower_recursion(float x, int n);

// 迭代求第 n 项斐波那契数
int fibonacci_iter(int n);

// 判断一个整数是否为素数
int is_prime(int num);

//冰雹猜想到达1的步数
int hailstone_steps(unsigned int n);

//三点求圆心
int get_circle_center(double x1, double y1, double x2, double y2, double x3, double y3,
                      double *cx, double *cy);

#endif