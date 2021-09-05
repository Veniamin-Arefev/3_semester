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

    char *dir;
    dir = getenv("XDG_RUNTIME_DIR");
    if (dir == NULL) {
        dir = getenv("TMPDIR");
    }
    if (dir == NULL) {
        dir = malloc(10);
        dir[0] = '/';
        dir[1] = 't';
        dir[2] = 'm';
        dir[3] = 'p';
        dir[4] = '\0';
    } else {
        int index = 0;
        while (dir[index++] != '=');
        dir = strdup(dir);
    }

    char *file = malloc(PATH_MAX);
    sprintf(file, "%s/%s", dir, "myprintfile");

    free(dir);

    int index = 0;
    while (1) {
        char *fileAttempt = malloc(PATH_MAX);
        sprintf(fileAttempt, "%s%05d.py", file, index++);

        int fd = open(fileAttempt, O_RDWR | O_CREAT | O_EXCL, 0700);
        free(fileAttempt);
        if (fd != -1) {
            close(fd);
            break;
        }
    }

    char *fileAttempt = malloc(PATH_MAX);
    sprintf(fileAttempt, "%s%05d.py", file, index - 1);

    FILE *filept = fopen(fileAttempt, "r+");
    fprintf(filept, "#!/usr/bin/env python3\nprint(%s", argv[1]);

    index = 2;
    while (argv[index] != NULL) {
        fprintf(filept, "*%s", argv[index]);
        index++;
    }
    fprintf(filept, ")\n");
    fclose(filept);

    char command[2 * PATH_MAX + 30];
    sprintf(command, "%s%s%s", fileAttempt, ";rm ", fileAttempt);
    free(file);

    execlp("/bin/sh", "sh", "-c", command, NULL);

    return 0;
}