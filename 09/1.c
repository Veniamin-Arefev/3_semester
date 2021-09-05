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

    int pid, status;
    if ((pid = fork()) == 0) {

        setbuf(stdout, NULL);
        int fd = open(argv[2], O_RDONLY);
        if (fd == -1) {
            _exit(42);
        }
        if (dup2(fd, 0) == -1) {
            _exit(42);
        }
        if (close(fd) == -1) {
            _exit(42);
        }

        fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (fd == -1) {
            _exit(42);
        }
        if (dup2(fd, 1) == -1) {
            _exit(42);
        }
        if (close(fd) == -1) {
            _exit(42);
        }

        fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (fd == -1) {
            _exit(42);
        }
        if (dup2(fd, 2) == -1) {
            _exit(42);
        }
        if (close(fd) == -1) {
            _exit(42);
        }

        execlp(argv[1], argv[1], NULL);
        _exit(42);
    } else {
        pid = waitpid(pid, &status, 0);
        printf("%d\n", status);
    }

    return 0;
}
