#include <stdio.h>
#include "math_utils.h"

int main() {
    // 测试 max3
    printf("max3(5, 8, 3) = %d\n", max3(5, 8, 3)); 

    // 测试 myPower_cycle
    printf("myPower_cycle(2, 3) = %.2f\n", myPower_cycle(2, 3));
    printf("myPower_cycle(2, -3) = %.2f\n", myPower_cycle(2, -3));

    // 测试 myPower_recursion
    printf("myPower_recursion(2, 3) = %.2f\n", myPower_recursion(2, 3));
    printf("myPower_recursion(2, -3) = %.2f\n", myPower_recursion(2, -3));

    // 测试 fibonacci_iter
    printf("fibonacci_iter(1) = %d\n", fibonacci_iter(1));
    printf("fibonacci_iter(6) = %d\n", fibonacci_iter(6));

    // 测试 is_prime
    printf("is_prime(17) = %d\n", is_prime(17));
    printf("is_prime(18) = %d\n", is_prime(18));

    // 测试 hailstone_steps
    printf("hailstone_steps(6) = %d\n", hailstone_steps(6));

    return 0;
}