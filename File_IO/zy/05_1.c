#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    /* ========== 方法1: fseeko + ftello (标准IO) ========== */
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    if (fseeko(fp, 0, SEEK_END) == -1) {
        perror("fseeko");
        return 1;
    }
    off_t size1 = ftello(fp);
    printf("【方法1】fseeko/ftello:  %ld 字节\n", (long)size1);

    /* ========== 方法2: 逐字节读取累加 ========== */
    rewind(fp);   // 等价于 fseek(fp, 0, SEEK_SET)
    long long size2 = 0;
    char buf[4096];
    int n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
        size2 += n;
    
    if (ferror(fp)) {
        perror("fread");
    } else {
        printf("【方法2】逐字节读取:     %lld 字节\n", size2);
    }
    fclose(fp);

    /* ========== 方法3: stat (通过路径) ========== */
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return 1;
    }
    printf("【方法3】stat(st_size):   %ld 字节\n", st.st_size);
    printf("        实际磁盘占用:    %ld 字节 (%ld 块)\n",
           st.st_blocks * 512, st.st_blocks);

    /* ========== 方法4: lseek (系统调用) ========== */
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    off_t size4 = lseek(fd, 0, SEEK_END);
    if (size4 == (off_t)-1) {
        perror("lseek");
        close(fd);
        return 1;
    }
    printf("【方法4】lseek:           %ld 字节\n", (long)size4);

    /* ========== 方法5: fstat (通过文件描述符) ========== */
    struct stat st2;
    if (fstat(fd, &st2) == -1) {
        perror("fstat");
    } else {
        printf("【方法5】fstat(st_size):  %ld 字节\n", st2.st_size);
    }
    close(fd);

    return 0;
}