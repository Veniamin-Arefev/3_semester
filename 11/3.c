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

int
main(int argc, char const *argv[]) {

    int N;
    int successExited = 0;
    sscanf(argv[1], "%d", &N);
    int *pids = calloc(N, sizeof(int *));

    int i;
    for (i = 0; i < N; i++) {
        if (argv[i+2] != NULL) {
            FILE *in = fopen(argv[i+2], "r");
            char command[PATH_MAX];
            fscanf(in, "%s", command);
            int pid;
            if ((pid = fork()) == 0) {
                execlp(command, command, NULL);
                _exit(1);
            } else {
                pids[i] = pid;
            }
        } else {
            N = i + 2;
            break;
        }
    }

    for (int b = 0; b < N; b++) {
        if (pids[b] != 0) {
            int status;
            waitpid(pids[b], &status, 0);
            if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
                successExited++;
            }
        }
    }
//    free(pids);

    i+=2;

    while (argv[i] != NULL) {
        FILE *in = fopen(argv[i], "r");
        char command[PATH_MAX];
        fscanf(in, "%s", command);
        int pid;
        if ((pid = fork()) == 0) {
            execlp(command, command, NULL);
            _exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
                successExited++;
            }
        }
        i++;
    }

    printf("%d\n", successExited);

    return 0;
}
