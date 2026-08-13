// 根据对文件位置调整的认识，将一个文件的读写位置调整到末尾10k处，
// 然后写入一些数据，比较该文件的实际大小和显示大小，体会“文件空洞”的概念。
// 思考：文件空洞有什么用？

#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 

int main(void)
{
    int fd = open("fileWithHole", O_CREAT|O_TRUNC|O_WRONLY);
    if(fd == -1)
    {
        perror("open失败");
        exit(0);
    }

    write(fd, "abc", 3);
    lseek(fd, 10*1024, SEEK_SET);
    write(fd, "xyz", 3);

    close(fd);

    return 0;
}


