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

    printf("%s\n", "INPUT_START");
    char input[5000];
    while (scanf("%s", input) > 0) {
        printf("%s\n", input);
    }
    printf("%s\n", "INPUT_END");

    return 0;
}
