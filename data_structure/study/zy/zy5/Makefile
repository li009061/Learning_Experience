# zy5 作业构建/验证入口
# 用法：
#   make         编译 01 和 02
#   make test    编译并运行 test.sh（两个程序都与独立 sort 结果交叉核对）
#   make clean   删除编译产物

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

TARGETS = 01 02

all: $(TARGETS)

01: 01.c
	$(CC) $(CFLAGS) -o $@ $<

02: 02.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	bash test.sh

clean:
	rm -f $(TARGETS)
