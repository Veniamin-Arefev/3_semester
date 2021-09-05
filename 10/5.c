#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int
main(int argc, char const *argv[]) {

    int pfd12[2];
    int pfd21[2];

    pipe(pfd12);
    pipe(pfd21);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    unsigned long long count;
    sscanf(argv[1], "%lld", &count);

    int pid, status;

    FILE *writefile12 = fdopen(pfd12[1], "w");
    setbuf(writefile12, NULL);
    FILE *readfile12 = fdopen(pfd12[0], "r");
    FILE *writefile21 = fdopen(pfd21[1], "w");
    setbuf(writefile21, NULL);
    FILE *readfile21 = fdopen(pfd21[0], "r");

    fprintf(writefile21, "%d ", 1);

    if ((pid = fork()) == 0) { // 1
        fclose(readfile12);
        close(pfd12[0]);
        fclose(writefile21);
        close(pfd21[1]);

        unsigned long long number;
        while (fscanf(readfile21, "%lld", &number) > 0) {
            if (number + 1 > count) {
                break;
            }
            printf("%d %lld\n", 1, number++);
            fprintf(writefile12, "%lld ", number);
        }

        fclose(writefile12);
        close(pfd12[1]);
        fclose(readfile21);
        close(pfd21[0]);
        _exit(0);
    } else {
        if ((pid = fork()) == 0) { // 2
            fclose(writefile12);
            close(pfd12[1]);
            fclose(readfile21);
            close(pfd21[0]);

            unsigned long long number;
            while (fscanf(readfile12, "%lld", &number) > 0) {
                if (number + 1 > count) {
                    break;
                }
                printf("%d %lld\n", 2, number++);
                fprintf(writefile21, "%lld ", number);
            }

            fclose(readfile12);
            close(pfd12[0]);
            fclose(writefile21);
            close(pfd21[1]);
            _exit(0);

        } else {
            fclose(writefile12);
            close(pfd12[1]);
            fclose(readfile21);
            close(pfd21[0]);
            fclose(readfile12);
            close(pfd12[0]);
            fclose(writefile21);
            close(pfd21[1]);

            wait(&status);
            wait(&status);
            printf("Done\n");
            exit(0);
        }
    }
}