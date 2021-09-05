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


int
main(int argc, char const *argv[]) {
    int pid, status;

    setbuf(stdin, NULL);
    for (int i = 0; i < 3; i++) {
        if ((pid = fork()) == 0) {
            setbuf(stdin, NULL);
            setbuf(stdout, NULL);
            int num;
            fscanf(stdin,"%d", &num);
            printf("%d %d\n", i + 1, num * num);
            _exit(0);
        }
    }
    while (wait(&status) != -1) {
    }
    return 0;
}
