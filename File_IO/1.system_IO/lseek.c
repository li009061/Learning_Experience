#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //a.txt的文件内容为abcdefghijk123
    // int fd = open("a.txt", O_RDONLY);

    // char buf[5];
    // bzero(buf, 5);
    // read(fd, buf, 4);
    // printf("%s\n", buf); // abcd

    // //--将从当前位置向后移动3个位置
    // lseek(fd, 3, SEEK_CUR);
    // read(fd, buf, 4);
    // printf("%s\n", buf); // hijk

    //--从最后向后移动10k的位置(中间是文件空洞)显示大小与实际大小不一致
    //使用du命令来查看
    int fd2 = open("xxx.tif", O_RDONLY);
    int size = lseek(fd2, 10000, SEEK_END);
    printf("size: %d\n", size);
}