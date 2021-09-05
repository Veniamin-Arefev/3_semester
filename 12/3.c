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

volatile int32_t counter = 0;

void mySigHandler(int sig) {
    if (sig == SIGUSR1) {
        counter += 5;
    } else {
        counter -= 4;
    }
    printf("%d %d\n", sig, counter);
    if (counter < 0) {
        _exit(0);
    }

}

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = mySigHandler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGUSR2, &(struct sigaction) {.sa_handler = mySigHandler, .sa_flags = SA_RESTART}, NULL);

//    signal(SIGUSR1, mySigHandler);
//    signal(SIGUSR2, mySigHandler);

    printf("%d\n", getpid());

    while (1) {
        pause();
    }

    return 0;
}
