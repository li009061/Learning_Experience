#ifndef FILEIO_H
#define FILEIO_H

#include <sys/types.h>

// 自报错封装：失败时库内部 perror 并返回 -1，调用者只需判断返回值
int     fio_open(const char *path, int flags);
void    fio_close(int fd);
ssize_t fio_read(int fd, void *buf, size_t n);
ssize_t fio_write(int fd, const void *buf, size_t n);

#endif // FILEIO_H
