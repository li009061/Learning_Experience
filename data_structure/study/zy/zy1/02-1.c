#include <stdio.h>

typedef struct {
    int a;
    int b; 
} Pair;

Pair josephus_last_two(int n) {

    if (n == 2) {
        Pair p = {0, 1};
        return p;
    }
    Pair prev = josephus_last_two(n - 1);    Pair curr;
    curr.a = (prev.a + 3) % n;
    curr.b = (prev.b + 3) % n;

    return curr;
}

int main() {
    int n = 13;
    Pair result = josephus_last_two(n);

    printf("从1开始编号：%d 和 %d\n", result.a + 1, result.b + 1);

    return 0;
}