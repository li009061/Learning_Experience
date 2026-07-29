// 给定一个 int 数据，要求尽快求出其二进制中 1 的位数。
// 提示：这是一道笔试题。这个问题的关键是要求“尽快”，可以考虑牺牲空间复杂度来换取时间复杂度。

#include <stdio.h>

// 预计算表：256 个字节，空间换时间
static unsigned char bit_count[256];

// 初始化查表（只需调用一次）
void init_table(void) {
    for (int i = 0; i < 256; i++) {
        bit_count[i] = (i & 1) + bit_count[i >> 1];
    }
}

// O(1) 查询
int count_ones(int n) {
    unsigned int x = n;
    return bit_count[x & 0xFF] +
           bit_count[(x >> 8) & 0xFF] +
           bit_count[(x >> 16) & 0xFF] +
           bit_count[(x >> 24) & 0xFF];
}

int main(void) {
    init_table();
    
    printf("%d\n", count_ones(0b10101010));  // 4
    printf("%d\n", count_ones(-1));           // 32
    
    return 0;
}