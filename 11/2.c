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

volatile int mode = 0;

void mySigHandler(int sig) {
    if (sig == SIGINT) {
        mode = 0;
    } else { //sig == SIGQUIT
        mode = 1;
    }
}

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    signal(SIGINT, mySigHandler);
    signal(SIGQUIT, mySigHandler);

    printf("%d\n", getpid());
    int32_t number = 0;
    int32_t sum = 0;
    while (scanf("%d", &number) > 0)  {
        int64_t newsum = 0;
        if (mode == 0) {
            newsum = ((int64_t)sum) + number;
        } else {
            newsum = ((int64_t)sum) * number;
        }
        sum = (int32_t) newsum;
        printf("%d\n", sum);
    }
}
