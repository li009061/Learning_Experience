#include "a.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int copyFile(FILE *fdSrc, FILE *fdDst){
    if(fdSrc == NULL || fdDst == NULL){
        perror("open file failed");
        return -1;
    }

    char buf[1024];
    int n, total = 0;
    while((n = fread(buf, 1, sizeof(buf), fdSrc)) > 0){
        fwrite(buf, 1, n, fdDst);
        total += n;
    }
    return total;
}

