//随机输入一个数，判断它是不是对称数（回文数）（如 3，121，12321，45254）。不能用字符串库函数

#include <stdio.h>

int main(int argc, char const *argv[])
{
    int n, m;
    scanf("%d", &n);
    m = n;

    int rn=0;
    while(n > 0)
    {
        // 1. 逐个拿到n的最低位
        int i = n%10;

        // 2. 将最低位拼接到rn末尾
        rn = rn*10+i;

        n /= 10;
    }

    if(m == rn)
        printf("是回文数\n");
    else
        printf("不是回文数\n");

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>   // for abs()

// int main(void)
// {
//     int n, m;
//     scanf("%d", &n);
//     m = abs(n);       // 取绝对值比较
//     n = abs(n);

//     int rn = 0;
//     while (n > 0) {
//         rn = rn * 10 + n % 10;
//         n /= 10;
//     }

//     if (m == rn)
//         printf("是回文数\n");
//     else
//         printf("不是回文数\n");

//     return 0;
// }
