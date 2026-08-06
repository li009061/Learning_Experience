#!/bin/bash
# 汉诺塔验证脚本（ad-hoc 断言，非正式测试框架）
# 核心：栈版 05_stack 的输出必须与递归参照实现逐字节一致
set -u
cd "$(dirname "$0")"
FAIL=0

# 临时递归参照版（即递归版 05.c 的逻辑，内嵌于此供对照）
cat > /tmp/v_rec.c <<'EOF'
#include <stdio.h>
long long step = 0;
void hanoi(int n, char from, char aux, char to)
{
    if (n == 1) { printf("第%lld步：%c -> %c\n", ++step, from, to); return; }
    hanoi(n - 1, from, to, aux);
    printf("第%lld步：%c -> %c\n", ++step, from, to);
    hanoi(n - 1, aux, from, to);
}
int main(void)
{
    int n;
    printf("请输入盘子个数 n：");
    scanf("%d", &n);
    hanoi(n, 'A', 'B', 'C');
    printf("--------------------------------\n");
    printf("总步数：%lld\n", step);
    printf("公式验证：2^%d - 1 = %lld\n", n, (1LL << n) - 1);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -o /tmp/v_rec /tmp/v_rec.c || { echo "FAIL: 参照版编译失败"; exit 1; }
gcc -Wall -Wextra -Werror -o /tmp/v_stack 05_stack.c || { echo "FAIL: 05_stack.c 编译失败"; exit 1; }
echo "PASS: 05_stack.c 编译通过（-Wall -Wextra -Werror）"

# 步数断言 n=3 / n=4
for n in 3 4; do
    cnt=$(/tmp/v_stack <<< "$n" | grep -oE '第[0-9]+步' | wc -l)
    exp=$(( (1 << n) - 1 ))
    if [ "$cnt" -eq "$exp" ]; then echo "PASS: n=$n 步数 = $exp"; else echo "FAIL: n=$n 期望 $exp 实际 $cnt"; FAIL=1; fi
done

# 与递归参照逐字节一致（n=5 和 n=6）
for n in 5 6; do
    if diff <(/tmp/v_rec <<< "$n") <(/tmp/v_stack <<< "$n") > /dev/null; then
        echo "PASS: n=$n 与递归参照输出完全一致（$(( (1 << n) - 1 )) 步）"
    else
        echo "FAIL: n=$n 与递归参照不一致"; FAIL=1
    fi
done

# 最后一步必须是 A -> C
echo "$(/tmp/v_stack <<< 5)" | grep '步：' | tail -1 | grep -q 'A -> C' \
    && echo "PASS: n=5 最后一步 A->C" || { echo "FAIL: 最后一步不是 A->C"; FAIL=1; }

# 边界
echo "$(/tmp/v_stack <<< 0)" | grep -q 'n 必须' && echo "PASS: n=0 被拒绝" || { echo "FAIL: n=0 未拒绝"; FAIL=1; }
echo "$(/tmp/v_stack <<< 21)" | grep -q 'n 太大' && echo "PASS: n=21 被拒绝" || { echo "FAIL: n=21 未拒绝"; FAIL=1; }

rm -f /tmp/v_rec /tmp/v_rec.c /tmp/v_stack
[ "$FAIL" -eq 0 ] && echo "=== ALL PASS ===" || echo "=== SOME FAILED ==="
exit "$FAIL"
