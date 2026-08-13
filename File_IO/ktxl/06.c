// 参照示例代码，打开目录并读取每一个目录项，达到如下要求：

// 不显示以 . 开头的隐藏文件。
// 不仅显示文件的名称，还要显示文件的大小。
// 提示：打开目录并不意味着进入目录，而获取目录里面的文件的信息必须要先进入目录，可以用函数 chdir() 进入目录。

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
#include <dirent.h>

int main(void)
{
    // 打开目录，获取目录指针
    DIR *dp = opendir("a");
    if(dp == NULL){
        perror("opendir failed");
        return 1;
    }

    chdir("a");

    // 读取每个目录项，并输出各个文件的名称
    struct dirent *ep;
    struct stat finfo;
    while(1)
    {
        ep = readdir(dp);

        // 读完了
        if(ep == NULL)
        {
            break;
        }
        if(ep->d_name[0]== '.')
            continue;

        bzero(&finfo, sizeof(finfo));
        stat(ep->d_name, &finfo);
        printf("%s %ld\n", ep->d_name, finfo.st_size);
    }
}