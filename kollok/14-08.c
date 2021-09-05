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
#include <signal.h>

int
main(int argc, char const *argv[]) {

    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    int pid = 0;
    if ((pid = fork()) == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    } else {
        if ((pid = fork()) == 0) {
            dup2(fd1[0], 0);
            dup2(fd2[1], 1);
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        } else {
            if ((pid = fork()) == 0) {
                dup2(fd2[0], 0);
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
                execlp(argv[3], argv[3], NULL);
                _exit(1);
            } else {
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);

                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }
    }

    return 0;
}
