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

int runAndCheck(const char *command) {
    int pid, status;
    if ((pid = fork()) == 0) {
        execlp(command, command, NULL);
        _exit(1);
    } else {
        pid = waitpid(pid, &status, 0);
        if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
            return 0;
        } else {
            return 1;
        }
    }

    return 0;
}


int
main(int argc, char const *argv[]) {

    if (runAndCheck(argv[1]) == 0) { //cmd1
        if (runAndCheck(argv[3]) == 0) { //cmd3
            return 0;
        } else {
            return 1;
        }
    } else {
        if (runAndCheck(argv[2]) == 0) { //cmd2
            if (runAndCheck(argv[3]) == 0) { //cmd3
                return 0;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    }


    return 0;
}
