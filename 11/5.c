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
#include <math.h>

volatile int maxnumber = 0;
volatile int lastPrime = 0;

void mySigHandler(int sig) {
    if (sig == SIGINT) {
        if (maxnumber == 3) {
            _exit(0);
        }
        printf("%d\n", lastPrime);
        maxnumber++;
    } else { //sig == SIGTERM
        _exit(0);
    }
}

int isPrime(int number) {
    if (number <= 1) {
        return 0;
    }
    if (number == 2) {
        return 1;
    }
    int max = (int) sqrt(number);
    for (int i = 2; i <= max; i++) {
        if (i >= number) {
            return 1;
        }
        if ((number % i) == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int low, high;

    scanf("%d", &low);
    scanf("%d", &high);
    printf("%d\n", getpid());

    signal(SIGINT, mySigHandler);
    signal(SIGTERM, mySigHandler);

    for (int i = low; i < high; i++) {
        if (isPrime(i)) {
            lastPrime = i;
        }
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    printf("%d\n", -1);
    _exit(0);

    return 0;
}
