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

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    int32_t count;
    int32_t modulo;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);

    int fd24[2];
    pipe(fd24);

    int pid1, pid2, pid;
    if ((pid1 = fork()) == 0) { //P1
        close(fd24[0]); // dont read reader
        int fd12[2];
        pipe(fd12);
        if ((pid = fork()) == 0) { //P2
            dup2(fd12[0], 0);
            dup2(fd24[1], 1);
            close(fd12[0]);
            close(fd12[1]);
            close(fd24[1]);
            execlp(argv[3], argv[3], NULL);
            exit(1);
        }
        dup2(fd12[1], 1);
        close(fd12[0]);
        close(fd12[1]);
        for (int i = 1; i < count + 1; i++) {
            u_int64_t number = i % modulo;
            number = (number * number) % modulo;
            u_int32_t numberOut = number;
            printf("%d ", numberOut);
        }
        close(1);
        close(fd24[1]);
        waitpid(pid, NULL, 0);
        _exit(0);
    }
    close(fd24[1]);

    if ((pid2 = fork()) == 0) { //P3
        if ((pid = fork()) == 0) { //P4
            dup2(fd24[0], 0);
            close(fd24[0]);
            execlp("sed", "sed", "s/ /\\n/g", NULL);
            exit(0);
        }
        close(fd24[0]);
        waitpid(pid, NULL, 0);
        exit(0);
    }
    close(fd24[0]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("0\n");

    return 0;
}