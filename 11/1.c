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

volatile int count = 0;

void mySigHandler(int sig) {
    if (count < 5) {
        printf("%d\n", count++);
    } else {
        _exit(0);
    }
}

int
main(int argc, char const *argv[]) {

    setbuf(stdout, NULL);

    sigaction(SIGHUP, &(struct sigaction) {.sa_handler = mySigHandler, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getpid());
    while (1)  {
        pause();
    }
}
