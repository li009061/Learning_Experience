#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int lcd = open("/dev/fb0", O_RDWR);
    if(lcd < 0)
    {
        perror("打开LCD失败");
        return 0;
    }

    char *p = mmap(NULL, 1024*600*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
    if(p == MAP_FAILED)
    {
        perror("映射内存失败");
        return 0;
    }

    uint32_t red = 0x00FF0000;
    for(int i=0; i<600; i++)
    {
        for(int j=0; j<1024; j++)
        {
            memcpy(p+j*4+1024*4*i, &red, 4);
        }
    }
    munmap(p, 1024*4*600);

    return 0;
}
