// 使用已学过的系统IO接口，测试默认情况下进程最多可以打开多少个文件。并在无法打开更多文件的情况下，将出错信息妥善输出。
// 提示：同一个文件可以以不同模式被多次打开。

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char const *argv[])
{

	// int count = 0;
	// while (1)
	// {
	// 	int fd1 = open(argv[1], O_RDONLY);
	// 	if(fd1 == -1){
	// 		perror("打开文件失败");
	// 		// return 0;
	// 		break;
	// 	}
	// 	count++;
	// }

	// printf("一共打开%d个\n", count);

	if(argc != 3){
		printf("用法:%s <源文件> <目标文件>\n", argv[0]);
		return -1;
	}

	int fd1 = open(argv[1], O_RDONLY);
		if(fd1 == -1){
			perror("打开文件失败");
			return 0;
		}

	int fd2 = open(argv[2], O_WRONLY, O_CREAT, O_TRUNC, 0644);
	if(fd2 == -1){
		perror("打开目标文件失败");
		return 0;
	}

	char buf[1024];
	while(1){
		int n = read(fd1, buf, sizeof(buf));

		if(n == -1){
			perroe("读取文件失败");
			break;
		}

		if(n == 0)
			break;

		
		// 试图将n个字节写入文件fd2中
        // 但实际的入d字节数是m
		char *p = buf;
		while (n > 0)
		{
			int  m = write(fd2, p, n);
			if(m == -1){
				perrow("写入文件失败");
				break;
			}
			p += m;
			n -=m;
		}
		
	}

	fclose(fd1);
	fclose(fd2);

	return 0;
}
