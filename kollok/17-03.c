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
    pipe(fd1);

    int pid;

    if ((pid = fork()) == 0) {
        if ((pid = fork()) == 0) {
            close(fd1[0]);
            int mypid = getpid();
            write(fd1[1], &mypid, sizeof(int));
            int mypid = getppid();
            write(fd1[1], &mypid, sizeof(int));
            close(fd1[1]);
            _exit(0);
        } else {
            if ((pid = fork()) == 0) {
                close(fd1[1]);
                int mypid, parentpid;
                read(fd1[0], &mypid, sizeof(int));
                read(fd1[0], &parentpid, sizeof(int));
                printf("%d %d\n", mypid, parentpid);
                close(fd1[0]);
                _exit(0);
            } else {
                close(fd1[0]);
                close(fd1[1]);
                wait(NULL);
                wait(NULL);
            }
        }
    } else {
        close(fd1[0]);
        close(fd1[1]);
        wait(NULL);
    }

    return 0;
}
