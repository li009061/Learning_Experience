// 1、(文件属性)
// 给定一个普通文件，使用尽可能多的方法，获取该文件的大小。

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
FILE *fp1 = fopen(“81lcd”,“rb”);
if (fp1 == NULL) {
perror(“打开文件失败”);
return 1;
}

// 3.使用lseek/fseek函数定位到文件末尾，然后获取文件位置即可获取文件大小。
fseek(fp1, 0, SEEK_END);
long long size = ftell(fp1);
printf("文件大小: %lld 字节\n", size);


//1.按字节读取文件，统计读取次数，直到读完为止。
long long M = 0;
int n;
char buf[1024];
fseek(fp1, 0, SEEK_SET);
while((n = fread(buf, 1, sizeof(buf), fp1))  > 0)
    M += n;

// 判断退出原因
if (feof(fp1)) {
    printf("文件最终大小: %lld 字节\n", M);
} else if (ferror(fp1)) {
    perror("读取文件出错");
}

fclose(fp1);
//2.使用stat函数直接获取文件大小（注意，若文件包含空洞则此信息不是文件真实大小）。
struct stat st;
if (stat(“81lcd”, &st) == -1) {
perror(“stat”);
return 1;
}
printf(“文件大小: %ld 字节\n”, st.st_size);

return 0;
}