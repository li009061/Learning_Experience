// 2、(目录操作、文件属性)

// 编写一个程序，令其功能向命令 “ls -l” 靠近。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

//将文件的类型和权限转换为字符串表示
void chmod_string(int data){
switch(data & S_IFMT){
case S_IFREG:
printf("- ");
break;
case S_IFDIR:
printf("d ");
break;
case S_IFCHR:
printf("c ");
break;
case S_IFBLK:
printf("b ");
break;
case S_IFLNK:
printf("l ");
break;
case S_IFSOCK:
printf("s ");
break;
case S_IFIFO:
printf("p “);
break;
default:
printf(”? ");
break;
}

char perm[] = {'r', 'w', 'x'};
int n = 0400;  // ----->八进制数 000 100 000 000
for(int i = 0; i < 9; i++){
    // 每次循环，将 n 右移 i 位，与 data 进行与操作，判断第 i 位是否为 1
    // 如果为 1，就打印对应的权限字符，否则打印 '-'
    printf("%c", (data & (n >> i)) ? perm[i % 3] : '-');
}
printf(" ");
}

int main(void)
{
DIR *dp = opendir(“…/ktxl”);
if(dp == NULL){
perror(“opendir failed”);
return 1;
}

chdir("../ktxl");
char ktxl[555];
bzero(ktxl, sizeof(ktxl));
getcwd(ktxl, sizeof(ktxl));
printf("%s\n", ktxl);

struct dirent *ep;
struct stat finfo;

while(1){
    ep = readdir(dp);
    if(ep == NULL)
        break;
    if(ep->d_name[0] == '.')
        continue;

    stat(ep->d_name, &finfo);
    chmod_string(finfo.st_mode);
    printf("%lu ", finfo.st_nlink);
    struct passwd *pw = getpwuid(finfo.st_uid);
    struct group  *gr = getgrgid(finfo.st_gid);
    printf("%s %s ", pw->pw_name, gr->gr_name);
    printf("%ld ", finfo.st_size);
    //
    printf("%.12s ", ctime(&finfo.st_mtime) + 4);
    printf("%s\n", ep->d_name);
}

return 0;
}

