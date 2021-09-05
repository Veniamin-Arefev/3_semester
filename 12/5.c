#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

int target_pid;
int fd;
unsigned char bit_number = 0;
unsigned char byte = 0;

void mySigHandlerReader(int sig) { //
    if (bit_number >= CHAR_BIT) {
        bit_number = 0;
        if (read(fd, &byte, sizeof(unsigned char)) < 1) {
            close(fd);
            kill(target_pid, SIGIO);
            _exit(0);
        }
    }
    unsigned char mask = 1u << bit_number;
    unsigned char bit = (byte & mask) >> bit_number;
    if (bit == 1) {
        kill(target_pid, SIGUSR1);
    } else {
        kill(target_pid, SIGUSR2);
    }
    bit_number++;

}

void mySigHandlerWriter(int sig) {
    if (bit_number == CHAR_BIT) {
        write(1, &byte, sizeof(unsigned char));
        byte = 0;
        bit_number = 0;
    }
    bit_number++;
    if (sig == SIGUSR1) { // 1
        unsigned char mask = 1u << (bit_number - 1);
        byte = byte | mask;
    } else if (sig == SIGUSR2) { // 0

    } else { // SIGIO / end
        _exit(0);
    }
    kill(target_pid, SIGALRM);

}

void mySigHandlerForPID(int sig, siginfo_t *siginfo, void *something) {
    target_pid = siginfo->si_pid;
//    printf("%d process set target to %d\n", getpid(), target_pid);
}

int
main(int argc, char const *argv[]) {

    int pid1, pid2;

    struct sigaction sigaction1;
    sigaction1.sa_flags = SA_SIGINFO | SA_RESETHAND;
    sigaction1.sa_sigaction = mySigHandlerForPID;

    sigaction(SIGINT, &sigaction1, NULL);

    signal(SIGUSR1, mySigHandlerWriter);
    signal(SIGUSR2, mySigHandlerWriter);
    signal(SIGIO, mySigHandlerWriter);
    if ((pid1 = fork()) == 0) {
        while (1) {
            pause();
        }
    }

    struct sigaction sigaction2;
    sigaction2.sa_handler = SIG_DFL;
    sigaction2.sa_flags = 0;
    sigaction(SIGINT, &sigaction2, NULL);

    signal(SIGALRM, mySigHandlerReader);
    if ((pid2 = fork()) == 0) {
        target_pid = pid1;
//        printf("%d target is %d pid\n", getpid(), target_pid);
//        printf("Send to %d our %d pid\n", target_pid, getpid());
        kill(target_pid, SIGINT);
        fd = open(argv[1], O_RDONLY);
        if (read(fd, &byte, sizeof(unsigned char)) < 1) {
            close(fd);
            kill(target_pid, SIGIO);
            _exit(0);
        }
        kill(getpid(), SIGALRM);
        while (1) {
            pause();
        }
    }


    wait(NULL);
    wait(NULL);

    return 0;
}
