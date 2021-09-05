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
#include <signal.h>
#include <wait.h>

int
main(int argc, char const *argv[]) {

    int pfd12[2];
    int pfd21[2];

    pipe(pfd12);
    pipe(pfd21);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int count;
    sscanf(argv[1], "%d", &count);

    int pid, status;

    int b = 1;
    write(pfd21[1], &b, sizeof(int));

    if ((pid = fork()) == 0) { // 1
        close(pfd12[0]);
        close(pfd21[1]);

        int number;
        while (read(pfd21[0], &number, sizeof(int)) > 0) {
            printf("%d %d\n", 1, number++);
            if (number == count) {
                break;
            }
            write(pfd12[1], &number, sizeof(int));
        }
        printf("Exited process 1\n");

        close(pfd12[1]);
        close(pfd21[0]);
        _exit(0);
    } else {
        if ((pid = fork()) == 0) { // 2
            close(pfd12[1]);
            close(pfd21[0]);

            int number;
            while (read(pfd12[0], &number, sizeof(int)) > 0) {
                printf("%d %d\n", 2, number++);
                if (number == count) {
                    break;
                }
                write(pfd21[1], &number, sizeof(int));
            }
            printf("Exited process 2\n");

            close(pfd12[0]);
            close(pfd21[1]);
            _exit(0);

        } else {
            close(pfd12[1]);
            close(pfd21[0]);
            close(pfd12[0]);
            close(pfd21[1]);

            wait(&status);
            wait(&status);
            printf("Done\n");
            exit(0);
        }
    }

    return 0;
}
