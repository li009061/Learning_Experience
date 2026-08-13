// 1、（标准IO）
// 编写一个程序，用标准IO提供的函数接口，实现文件的拷贝。


文件拷贝
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void copy_file(FILE * pf1, FILE *pf2){
char buf[1024];
while(1){
size_t n = fread(buf, 1, 1024, pf1);
if(n == 0){
if(feof(pf1))
break;
else if(ferror(pf1)){
perror(“文件读取失败”);
return;
}
}
if(fwrite(buf, 1, n, pf2) != n){
perror(“文件写入失败”);
return;
}
}
}

int main(int argc, char *argv[])
{
// 检查参数是否正确
if(argc != 3){
fprintf(stderr, “Usage: %s src dst\n”, argv[0]);
return -1;
}

//打开文件
FILE *fp1 = fopen(argv[1], "r");
FILE *fp2 = fopen(argv[2], "w");

//判断文件是否打开成功
if(fp1 == NULL){
    fprintf(stderr, "open %s failed", argv[1]);
    return -1;
}
if(fp2 == NULL){
    fprintf(stderr,"open %s failed", argv[2]);
    return -1;
}

//拷贝文件
copy_file(fp1,fp2);

fclose(fp1);
fclose(fp2);

return 0;
}