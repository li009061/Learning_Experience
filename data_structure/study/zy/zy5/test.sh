#!/bin/bash
# zy5 验证脚本（ad-hoc 断言）
# 核心：程序输出的第 K 名成绩 必须与 awk+sort 独立计算的第 K 名成绩一致
# 对 01.c（快排）和 02.c（快速选择）分别验证
set -u
cd "$(dirname "$0")"
FAIL=0

for prog in 01 02; do
    echo "--- 验证 $prog.c ---"
    if ! gcc -Wall -Wextra -Werror -o "/tmp/v_$prog" "$prog.c" 2>"/tmp/v_$prog.err"; then
        echo "FAIL: $prog.c 编译失败"; cat "/tmp/v_$prog.err"; FAIL=1
        continue
    fi
    echo "PASS: $prog.c 编译通过（-Wall -Wextra -Werror）"

    for K in 1 5 26 52; do
        exp=$(awk '{print $NF}' record.txt | sort -n | sed -n "${K}p")
        out=$(/tmp/v_$prog <<< "$K")
        if echo "$out" | grep -q "成绩 $exp 秒"; then
            echo "PASS: $prog K=$K → 成绩 $exp 秒"
        else
            echo "FAIL: $prog K=$K 期望成绩 $exp，实际输出: $out"; FAIL=1
        fi
    done

    out1=$(/tmp/v_$prog <<< 1)
    echo "$out1" | grep -qE '提莫|布里茨' && echo "PASS: $prog 第 1 名姓名正确（提莫/布里茨）" || { echo "FAIL: $prog 第 1 名姓名异常"; FAIL=1; }

    out0=$(printf "0\n3\n" | /tmp/v_$prog)
    echo "$out0" | grep -q '输入无效' && echo "PASS: $prog K=0 被拒绝" || { echo "FAIL: $prog K=0 未被拒绝"; FAIL=1; }
    out53=$(printf "53\n3\n" | /tmp/v_$prog)
    echo "$out53" | grep -q '输入无效' && echo "PASS: $prog K=53 被拒绝" || { echo "FAIL: $prog K=53 未被拒绝"; FAIL=1; }
done

rm -f /tmp/v_01 /tmp/v_02 /tmp/v_01.err /tmp/v_02.err
[ "$FAIL" -eq 0 ] && echo "=== ALL PASS ===" || echo "=== SOME FAILED ==="
exit "$FAIL"
