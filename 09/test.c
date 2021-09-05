#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char const *argv[]) {

    char *file = malloc(PATH_MAX);
    sprintf(file, "%s/%s", "kek", "myprintfile");
    sprintf(file, "%s/%s", file, file);
    printf("%s\n", file);

    return 0;
}
