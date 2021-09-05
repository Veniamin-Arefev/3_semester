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

    int pid;

    if ((pid = fork()) == 0) {
        close(fd1[0]);
        close(fd2[1]);
        int mypid = getpid();
        write(fd1[1], &mypid, sizeof(int));
        read(fd2[0], &mypid, sizeof(int));
        close(fd1[1]);
        close(fd2[0]);
        _exit(0);
    } else {
        if ((pid = fork()) == 0) {
            close(fd1[1]);
            close(fd2[0]);
            int mypid = getpid();
            write(fd2[1], &mypid, sizeof(int));
            read(fd1[0], &mypid, sizeof(int));
            close(fd2[1]);
            close(fd1[0]);
            _exit(0);
        } else {
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
