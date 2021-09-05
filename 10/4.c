#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int pidCount;
int pidLength;
int *pids;

void errorExiting(void) {
    for (int i = 0; i < pidLength; i++) {
        if (pids[i] != 0) {
            kill(pids[i], SIGKILL);
            waitpid(pids[i], NULL, 0);
        }
    }
    _exit(1);
}

void myClose(int fd) {
    if (close(fd) == -1) {
        errorExiting();
    }
}

void addPid(int pid) {
    if (pidCount + 1 >= pidLength) {
        pidLength += 5; // just add 5 more spaces for pids array
        int *newpids = realloc(pids, pidLength * sizeof(int));
        if (newpids == NULL) {
            pids[pidLength - 1] = pid;
            errorExiting();
        } else {
            pids = newpids;
            for (int i = pidLength - 5; i < pidLength; i++) { // set 0 all new array spots
                pids[i] = 0;
            }
        }
    }
    int i = -1;
    while (pids[++i] != 0);
    pids[i] = pid;
    pidCount++;
}

void checkZombieProcesses(void) {
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

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    pidLength = 5; // just init value
    pidCount = 0; // at start we have 0 child processes
    pids = calloc(pidLength, sizeof(int));

    if (pids == NULL) {
        _exit(1);
    }

    int pfd[4];
    int pid;

    if (argc == 1) { // empty command line
        _exit(0);
    }

    if (argc == 2) { // only cmd1
        if ((pid = fork()) == 0) { // cmd1
            free(pids); // allocated by *alloc
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else {
            if (pid == -1) {
                errorExiting();
            }
            addPid(pid);
            checkZombieProcesses();
        }
        if (waitpid(pid, NULL, 0) == -1) {
            errorExiting();
        }
        _exit(0);
    }

    if (pipe(pfd) == -1) {
        errorExiting();
    }

    int index = 0; // index for counting processes from cmd2 to cmd<N-1>
    if ((pid = fork()) == 0) { // cmd1
        free(pids); // allocated by *alloc
        dup2(pfd[1], 1);
        close(pfd[0]);
        close(pfd[1]);
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        if (pid == -1) {
            errorExiting();
        }
        addPid(pid);
        checkZombieProcesses();
    }

    for (index++; index < (argc - 1) - 1; index++) { // index == 0 at start
        pipe(&pfd[index * 2 % 4]);
        if ((pid = fork()) == 0) {
            free(pids); // allocated by *alloc
            dup2(pfd[(index - 1) * 2 % 4], 0);
            dup2(pfd[index * 2 % 4 + 1], 1);
            close(pfd[0]);
            close(pfd[1]);
            close(pfd[2]);
            close(pfd[3]);
            execlp(argv[index + 1], argv[index + 1], NULL);
            _exit(1);
        } else {
            if (pid == -1) {
                errorExiting();
            }
            addPid(pid);
            myClose(pfd[(index - 1) * 2 % 4]);
            myClose(pfd[(index - 1) * 2 % 4 + 1]);
            checkZombieProcesses();
        }
    }

    if ((pid = fork()) == 0) { // for cmdN     -- at least cmd2 exists(and number >= 1)
        free(pids); // allocated by *alloc
        dup2(pfd[(index - 1) * 2 % 4], 0);
        close(pfd[(index - 1) * 2 % 4]);
        close(pfd[(index - 1) * 2 % 4 + 1]);
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        if (pid == -1) {
            errorExiting();
        }
        addPid(pid);
        myClose(pfd[(index - 1) * 2 % 4]);
        myClose(pfd[(index - 1) * 2 % 4 + 1]);
        checkZombieProcesses();
    }
    for (int i = 0; i < pidLength; i++) {
        if (pids[i] != 0) {
            int temp = waitpid(pids[i], NULL, 0);
            if (temp == -1) {
                errorExiting();
            }
        }
    }

    return 0;
}
