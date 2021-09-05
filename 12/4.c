#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

int
main(int argc, char const *argv[]) {

    int fd = open(argv[1], O_CREAT | O_RDWR, 0751u);
    close(fd);

    FILE *filept = fopen(argv[1], "w+");
    fprintf(filept, "#!/usr/bin/env python3\n");
    fprintf(filept, "b = int(input())\n");
    fprintf(filept, "s = b*(b+1)\n");
    fprintf(filept, "s = s // 2\n");
    fprintf(filept, "print(int(s%%%s))\n", argv[2]);

    fclose(filept);

    return 0;
}