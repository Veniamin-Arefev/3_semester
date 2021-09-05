#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int fd[2];
int maxnumber = 0;
int target_pid = 0;
int process_number = 0;
FILE *writefile;
FILE *readfile;

void mySignalHandler(int sig) {
//    printf("Entered %d pid signal handler\n", getpid());
    int number;
    if (fscanf(readfile, "%d", &number) > 0) {
        if (target_pid == 0) {
            target_pid = number;
//            printf("Process %d set target to %d\n", process_number, target_pid);
        } else {
            if (number == -1) {
//                printf("Got -1; Exiting...\n");
                fclose(writefile);
                fclose(readfile);
                close(fd[0]);
                close(fd[1]);
                _exit(0);
            }
            if (number < maxnumber) {
                printf("%d %d\n", process_number, number++);
//                printf("Printed%d\n", fprintf(writefile, "%d ", number));
                fprintf(writefile, "%d ", number);
            } else {
                number = -1;
                fprintf(writefile, "%d ", number);
                fclose(writefile);
                fclose(readfile);
                close(fd[0]);
                close(fd[1]);
//                printf("Got %d number\n", number);
                kill(target_pid, SIGUSR1);
                _exit(0);
            }
        }
//        printf("Sended from %d to %d pid ping signal\n", getpid(), target_pid);
//        printf("Can send? - %d \n", kill(target_pid, 0));
        kill(target_pid, SIGUSR1);
    } else { // ERROR
        printf("ERROR\n");
        _exit(1);
    }
}


int
main(int argc, char const *argv[]) {

    pipe(fd);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    sscanf(argv[1], "%d", &maxnumber);

    int pid1, pid2, status;

    readfile = fdopen(fd[0], "r");
    setbuf(readfile, NULL);
    writefile = fdopen(fd[1], "w");
    setbuf(writefile, NULL);

    signal(SIGUSR1, mySignalHandler);
    if ((pid1 = fork()) == 0) {
        process_number = 1;
        while (1) {
            pause();
        }
    } else {
        if ((pid2 = fork()) == 0) {
            process_number = 2;
            while (1) {
                pause();
            }
        } else {
            signal(SIGUSR1, SIG_IGN);
            fprintf(writefile, "%d ", pid2);
            fprintf(writefile, "%d ", pid1);
            fprintf(writefile, "%d ", 1);

//            printf("%d %d\n", pid1, pid2);

            fclose(writefile);
            fclose(readfile);
            close(fd[0]);
            close(fd[1]);

            kill(pid1, SIGUSR1);

//            sleep(6);

            waitpid(pid1, &status, 0);
            waitpid(pid2, &status, 0);
            printf("Done\n");
            exit(0);
        }
    }

    return 0;
}