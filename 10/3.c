#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int pfd[2];
    pipe(pfd);

    int pid, pid2, status;
    if ((pid2 = fork()) == 0) { // process for handle () expression

        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);

        if ((pid = fork()) == 0) { //cmd 1
            int fd = open(argv[4], O_RDONLY, 0);
            dup2(fd, 0);
            close(fd);

            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
                if ((pid2 = fork()) == 0) { // cmd2
                    execlp(argv[2], argv[2], NULL);
                    _exit(1);
                } else {
                    waitpid(pid2, &status, 0);
                }
            }
        }
        _exit(0);
    }
    if ((pid = fork()) == 0) { // cmd3
        close(pfd[1]);
        int fd = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0600);
        dup2(fd, 1);
        close(fd);

        dup2(pfd[0], 0);
        close(pfd[0]);

        execlp(argv[3], argv[3], NULL);
        _exit(1);
    } else {
        close(pfd[0]);
        close(pfd[1]);
        waitpid(pid2, &status, 0); // () expression
        waitpid(pid, &status, 0); // cmd3
    }

    _exit(0);
}