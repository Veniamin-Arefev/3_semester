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
        close(fd[1]);
        waitpid(pid, &status, 0);
        time_t currenttime;
        read(fd[0], &currenttime, sizeof(time_t));
        close(fd[0]);
        struct tm *time = localtime(&currenttime);
        printf("Y:%04d\n", time->tm_year+1900);
        _exit(0);
    } else {
        if ((pid = fork()) > 0) {
            close(fd[1]);
            waitpid(pid, &status, 0);
            time_t currenttime;
            read(fd[0], &currenttime, sizeof(time_t));
            close(fd[0]);
            struct tm *time = localtime(&currenttime);
            printf("M:%02d\n", time->tm_mon+1);
            _exit(0);
        } else {
            time_t currenttime = time(NULL);
            write(fd[1], &currenttime, sizeof(time_t));
            write(fd[1], &currenttime, sizeof(time_t));
            write(fd[1], &currenttime, sizeof(time_t));
            read(fd[0], &currenttime, sizeof(time_t));
            close(fd[0]);
            close(fd[1]);
            struct tm *time = localtime(&currenttime);
            printf("D:%02d\n", time->tm_mday);
            _exit(0);
        }
    }

    return 0;
}