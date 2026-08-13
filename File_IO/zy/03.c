// （标准IO缓冲区）
// 编写一个程序，检测标准IO的缓冲区的大小。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int main(int argc, char *argv[])
{
if(argc != 2){
fprintf(stderr, “Usage: %s <filename>\n”, argv[0]);
return 1;
}

FILE *fp = fopen(argv[1], "w");
if(fp == NULL){
    perror("fopen failed");
    return 1;
}
char buf[] = {"0123456789"};
for(int i = 0; i < 5000; i++){
    fputc(buf[i%10], fp);
}

pause();
}



