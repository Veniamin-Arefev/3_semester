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
    int status;
    int N;
    scanf("%d", &N);
    setbuf(stdout, NULL);
    printf("%d", 1);
    for (int i = 2; i < N+1; i++) {
        if (fork() == 0) {
            setbuf(stdout, NULL);
            if (i == 1) {
                printf("%d", i);
            } else {
                printf(" %d", i);
            }
            _exit(0);
        } else {
            wait(&status);
        }
    }

    printf("\n");
    return 0;
}
