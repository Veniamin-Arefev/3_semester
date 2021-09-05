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
    if ((pid =fork()) == 0) {
        if ((pid =fork()) == 0) {
            printf("3 ");
            return 0;
        } else {
            waitpid(pid, &status, 0);
            printf("2 ");
            return 0;
        }
    } else {
        waitpid(pid, &status, 0);
        printf("1");
    }
    printf("\n");
    return 0;
}
