#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>


int main(int argc, char const *argv[]) // ./a.out xxxx
{
    if(argc != 2)
    {
        fprintf(stderr, "请指定一个文件\n");
        return 0;
    }

    // 取得指定文件的属性信息
    struct stat st;
    stat(argv[1], &st);

    // 输出文件的设备号
    if(S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode))
        printf("%d, %d\n", major(st.st_rdev), minor(st.st_rdev));

    else if(S_ISREG(st.st_mode) || S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode))
        printf("%d, %d\n", major(st.st_dev), minor(st.st_dev));

    return 0;
}
 