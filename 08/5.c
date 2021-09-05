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

int func(void) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int pid, status, num;
    if ((pid = fork()) > 0) {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) == 1 && WEXITSTATUS(status) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else if (pid == 0) {
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
        if (scanf("%d", &num) == 1) {
            if (func() == 0) {
                setbuf(stdout, NULL);
                printf("%d\n", num);
                _exit(0);
            } else {
                _exit(1);
            }
        } else {
            _exit(0);
        }

    } else { // pid == -1
        return 1;
    }

    return 0;
}

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    if (func() == 1) {
        printf("-1");
    }

    return 0;
}
