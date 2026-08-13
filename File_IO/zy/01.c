// 1、
// （系统IO）
// 编写一个程序，用系统IO提供的函数接口，实现文件的拷贝。


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
//打开两个文件
int fd1 = open(“a.txt”, O_RDONLY);
if(fd1 == -1){
perror(“文件打开失败”);
return -1;
}

int fd2 = open("b.txt", O_WRONLY | O_CREAT | O_TRUNC , 0644);
if(fd2 == -1){
    perror("文件打开失败");
    return -1;
}

//读取一个文件
char buf[256];
char *p = buf;
int n, m;
while((n = read(fd1, p, sizeof(buf))) > 0){
    p = buf;
    //写入一个文件
    while(n >0){
        m = write(fd2, p, n);
        if(m == -1){
            perror("写入失败");
            return -1;
        }
        p += m;
        n -= m;
    }

    if (n == -1){
        perror("读取失败");
    }
}

close(fd1);
close(fd2);
return 0;
}