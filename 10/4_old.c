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

int pfd[6];
int pid[3] = {0, 0, 0};

void errorExiting();

void closeAllPipes() {
    for (int i = 0; i < 6; ++i) {
        close(pfd[i]);
    }
}

void myClose(int fd) {
    if (close(fd) == -1) {
        errorExiting();
    }
}

void closeAllPipesWithCheck() {
    for (int i = 0; i < 6; ++i) {
        myClose(pfd[i]);
    }
}

void killAllProcess() {
    for (int i = 0; i < 3; ++i) {
        if (pid[i] != 0) {
            kill(pid[i], SIGKILL);
            int status = 0;
            waitpid(pid[i], &status, 0);
        }
    }
}

void errorExiting() {
    fprintf(stderr, "Error occurred\n");
    closeAllPipes();
    killAllProcess();
    _exit(1);
}

int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    if (argc == 1) { // empty command line
        _exit(0);
    }

    if (pipe(pfd) == -1) {
        errorExiting();
    }
    if (pipe(pfd + 2) == -1) {
        errorExiting();
    }
    if (pipe(pfd + 4) == -1) {
        errorExiting();
    }

    int index = 0;
    if ((pid[index % 3] = fork()) == 0) {
        dup2(pfd[index * 2 % 6 + 1], 1);
        closeAllPipes();
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        fprintf(stderr, "created %d pid process\n", pid[index % 3]);
        if (pid[index % 3] == -1) {
            errorExiting();
        }
    }
    if (argc == 2) { // only cmd1
        if (dup2(0, pfd[index * 2 % 6]) == -1) {
            errorExiting();
        }
        closeAllPipesWithCheck();
        char c;
        while ((scanf("%c", &c)) > 0) {
            printf("%c", c);
        }
        waitpid(pid[0], NULL, 0);
        _exit(0);
    }

    for (index++; index < (argc - 1) - 1; index++) { // index == 0 at start
        if ((pid[index % 3] = fork()) == 0) {
            dup2(pfd[(index - 1) * 2 % 6], 0);
            dup2(pfd[index * 2 % 6 + 1], 1);
            closeAllPipes();
            execlp(argv[index + 1], argv[index + 1], NULL);
            _exit(1);
        } else {
            fprintf(stderr, "created %d pid process and index is %d\n", pid[index % 3], index);
            myClose(pfd[(index - 1) * 2 % 6]);
            myClose(pfd[(index - 1) * 2 % 6 + 1]);
            if (pipe(&pfd[(index - 1) * 2 % 6]) == -1) {
                errorExiting();
            }
            if (pid[index % 3] == -1) {
                errorExiting();
            }
            if (index >= 2) { //should close (index - 2) * 2 % 6 descriptors
                waitpid(pid[index - 2 % 3], NULL, 0);
                pid[index - 2 % 3] = 0;
            }
//            sleep(10);
        }
    }

    if ((pid[index % 3] = fork()) == 0) { // for cmdN     -- at least cmd2 exists(and number >= 2)
        dup2(pfd[(index - 1) * 2 % 6], 0);
        closeAllPipes();
        execlp(argv[index + 1], argv[index + 1], NULL);
        _exit(1);
    } else {
        fprintf(stderr, "created %d pid process\n", pid[index % 3]);
        if (pid[index % 3] == -1) {
            errorExiting();
        }
        closeAllPipesWithCheck();
        for (int i = 0; i < 3; i++) {
            if (pid[i] != 0) {
                int status = 0;
                fprintf(stderr, "wait for %d pid\n", pid[i]);
                waitpid(pid[i], &status, 0);
            }
        }
    }
//    sleep(15);

    return 0;
}
