# 汉诺塔作业构建/验证入口
# 用法：
#   make           编译 05_stack 和 hanoi_game
#   make test      编译并运行 test.sh 断言（栈版 vs 递归参照逐字节一致）
#   make clean     删除编译产物（不动 a.out）

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

TARGETS = 05_stack hanoi_game

all: $(TARGETS)

05_stack: 05_stack.c
	$(CC) $(CFLAGS) -o $@ $<

hanoi_game: hanoi_game.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	bash test.sh

clean:
	rm -f $(TARGETS)
