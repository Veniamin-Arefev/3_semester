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

int pidCount;
int pidLength;
int *pids;

void errorExiting();

void myClose(int fd) {
    if (close(fd) == -1) {
        errorExiting();
    }
}

void killAllProcess() {
    for (int i = 0; i < pidLength; i++) {
        if (pids[i] != 0) {
            kill(pids[i], SIGKILL);
            waitpid(pids[i], NULL, 0);
        }
    }
}

void errorExiting() {
    fprintf(stderr, "Error occurred\n");
    killAllProcess();
    _exit(1);
}

void addPid(int pid) {
    if (pidCount + 1 >= pidLength) {
        int *newpids = realloc(pids, pidLength+=5);
        if (newpids == NULL) {
            errorExiting();
        } else {
            pids = newpids;
        }
    }
    int i = -1;
    while (pids[++i] != 0);
    pids[i] = pid;
}

void checkZombieProcesses() {
    for (int i = 0; i < pidLength; i++) {
        if (pids[i] != 0) {
            int temp = waitpid(pids[i], NULL, WNOHANG);
            if (temp > 0) {
                pids[i] = 0;
                pidCount--;
            }
        }
    }
}

int
main(int argc, char const *argv[]) {

    pidLength = 10;
    pidCount = 0;
    pids = calloc(pidLength, sizeof(int));

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int pfd[4];
    int pid;

    if (argc == 1) { // empty command line
        _exit(0);
    }

    if (pipe(pfd) == -1) {
        errorExiting();
    }

    int index = 0;
    if ((pid = fork()) == 0) { // cmd1
        dup2(pfd[1], 1);
        close(pfd[0]);
        close(pfd[1]);
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        addPid(pid);
        checkZombieProcesses();
        fprintf(stderr, "created %d pid process\n", pid);
        if (pid == -1) {
            errorExiting();
        }
    }
    if (argc == 2) { // only cmd1
        if (dup2(0, pfd[0]) == -1) {
            errorExiting();
        }
        myClose(pfd[0]);
        myClose(pfd[1]);
        char c;
        while ((scanf("%c", &c)) > 0) {
            printf("%c", c);
        }
        waitpid(pid, NULL, 0);
        _exit(0);
    }

    for (index++; index < (argc - 1) - 1; index++) { // index == 0 at start
        pipe(&pfd[index * 2 % 4]);
        if ((pid = fork()) == 0) {
            dup2(pfd[(index - 1) * 2 % 4], 0);
            dup2(pfd[index * 2 % 4 + 1], 1);
            close(pfd[0]);
            close(pfd[1]);
            close(pfd[2]);
            close(pfd[3]);
            execlp(argv[index + 1], argv[index + 1], NULL);
            _exit(1);
        } else {
            addPid(pid);
            checkZombieProcesses();
            fprintf(stderr, "created %d pid process and index is %d\n", pid, index);
            myClose(pfd[(index - 1) * 2 % 4]);
            myClose(pfd[(index - 1) * 2 % 4 + 1]);
            if (pid == -1) {
                errorExiting();
            }
            sleep(1);
        }
    }

    if ((pid = fork()) == 0) { // for cmdN     -- at least cmd2 exists(and number >= 2)
        dup2(pfd[(index - 1) * 2 % 4], 0);
        close(pfd[(index - 1) * 2 % 4]);
        close(pfd[(index - 1) * 2 % 4 + 1]);
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        close(pfd[(index - 1) * 2 % 4]);
        close(pfd[(index - 1) * 2 % 4 + 1]);
        addPid(pid);
        checkZombieProcesses();
        fprintf(stderr, "created %d pid process\n", pid);
        if (pid == -1) {
            errorExiting();
        }
//        for (int i = 0; i < 3; i++) {
//            if (pid[i] != 0) {
//                int status = 0;
//                fprintf(stderr, "wait for %d pid\n", pid[i]);
//                waitpid(pid[i], &status, 0);
//            }
//        }
    }
    fprintf(stderr, "Start cleanup\n");
    sleep(10);
    for (int i = 0; i < pidLength; i++) {
        if (pids[i] != 0) {
            int temp = waitpid(pids[i], NULL, 0);
            if (temp > 0) {
                fprintf(stderr, "Closed %d process\n", temp);
            }
            if (temp == -1) {
                errorExiting();
            }
        }
    }
    fprintf(stderr, "End cleanup\n");
    sleep(10);


    return 0;
}
