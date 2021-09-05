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

int mysys(const char *str) {

    int pid, status;
    if ((pid = fork()) == 0) {
        execlp("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else {
        if (pid == -1) {
            return -1;
        }
        pid = waitpid(pid, &status, 0);
        if (WIFEXITED(status) != 0) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status) != 0) {
            return 128 + WTERMSIG(status);
        }
        printf("%d\n", status);
    }
    //error
    return -1;
}
