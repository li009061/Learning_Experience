#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "fileio.h"

// 自报错：出错时自己 perror 并返回 -1，调用者不用查 errno
int fio_open(const char *path, int flags)
{
    int fd = open(path, flags, 0644);
    if(fd < 0){
        perror("fio_open");
        return -1;
    }
    return fd;
}

void fio_close(int fd)
{
    if(fd >= 0)
        close(fd);
}

ssize_t fio_read(int fd, void *buf, size_t n)
{
    ssize_t ret = read(fd, buf, n);
    if(ret < 0)
        perror("fio_read");
    return ret;
}

ssize_t fio_write(int fd, const void *buf, size_t n)
{
    ssize_t ret = write(fd, buf, n);
    if(ret < 0)
        perror("fio_write");
    return ret;
}
