#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char const *argv[]) {

    for (int i = 1; i < argc; i++) {
        long long b = strlen(argv[i]);
        if (b > 0) {
            int exit = 0;
            for (int j = 0; j < b; j++) {
                if (!(argv[i][j] >= 'a' && argv[i][j] <= 'z')) {
                    exit = 1;
                    break;
                }
            }
            if (exit) {
                continue;
            }
            for (int j = 0; j < b/2; j++) {
                if (argv[i][j] != argv[i][b-1-j]) {
                    exit = 1;
                    break;
                }
            }
            if (exit) {
                continue;
            }
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}
