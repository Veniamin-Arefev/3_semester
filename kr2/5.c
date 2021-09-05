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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

int
main(int argc, char const *argv[]) {

    int fd1br[2];
    pipe(fd1br);

    int pidbr = 0;
    int pid1 = 0;
    int pid2 = 0;
    int pid3 = 0;
    int pid4 = 0;

    if ((pid1 = fork()) == 0) { //cmd1
        close(fd1br[0]);
        dup2(fd1br[1], 1);
        close(fd1br[1]);
        execlp(argv[1], argv[1], NULL);
        _exit(0);
    }

    close(fd1br[1]);
    int fdbr4[2];
    pipe(fdbr4);

    if ((pidbr = fork()) == 0) { // ()
        dup2(fd1br[0], 0);
        close(fd1br[0]);
        dup2(fdbr4[1], 1);
        close(fdbr4[1]);
        close(fdbr4[0]);
        if ((pid2 = fork()) == 0) { //cmd2
            int fd = open(argv[5], O_WRONLY | O_CREAT | O_TRUNC, 0600);
            dup2(fd, 2);
            close(fd);
            execlp(argv[2], argv[2], NULL);
            _exit(0);
        } else {
            int status;
            waitpid(pid2, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                if ((pid3 = fork()) == 0) { // cmd3
                    execlp(argv[3], argv[3], NULL);
                    _exit(0);
                } else {
                    waitpid(pid3, &status, 0);
                }
            }
        }
        _exit(0);
    }

    close(fd1br[0]);
    close(fdbr4[1]);

    if ((pid4 = fork()) == 0) { //cmd4
        dup2(fdbr4[0], 0);
        close(fdbr4[0]);
        execlp(argv[4], argv[4], NULL);
        _exit(0);
    }

    close(fdbr4[0]);
    waitpid(pid1, NULL, 0);
    waitpid(pidbr, NULL, 0);
    waitpid(pid4, NULL, 0);

    _exit(0);
}