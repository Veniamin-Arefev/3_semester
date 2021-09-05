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

    setbuf(stdout, NULL);

    int fd[2];
    pipe(fd);
    int pid, status;
    if ((pid = fork()) > 0) {
        close(fd[0]);
        int32_t number;
        while (scanf("%d", &number) > 0) {
            write(fd[1], &number, sizeof(time_t));
        }
        close(fd[1]);
        waitpid(pid, &status, 0);
        _exit(0);
    } else {
        if ((pid = fork()) > 0) {
            close(fd[1]);
            close(fd[0]);
            waitpid(pid, &status, 0);
            _exit(0);
        } else {
            close(fd[1]);
            int32_t number;
            int64_t sum = 0;
            while (read(fd[0], &number, sizeof(time_t)) > 0) {
                sum += number;
            }
            printf("%lld\n", sum);
            close(fd[0]);
            _exit(0);
        }
    }

    return 0;
}
