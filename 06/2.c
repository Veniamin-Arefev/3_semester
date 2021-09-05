#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

int
main(int argc, char const *argv[]) {


    DIR *d = opendir(argv[1]);
    if (!d) {
        exit(1);
    }
    struct dirent *pd;
    long long count = 0;
    while ((pd = readdir(d))) {
        if (!strcmp(pd->d_name, ".") || !strcmp(pd->d_name, "..")) continue;
        char path[PATH_MAX];
        sprintf(path, "%s/%s", argv[1], pd->d_name);
        struct stat stbuf;
        if (stat(path, &stbuf) != 0) continue;
        int n = strlen(pd->d_name);
        if (n < 4) {
            continue;
        }
        if (strcmp(pd->d_name + n - 4, ".exe") != 0) {
            continue;
        }
        if (((stbuf.st_mode & S_IFMT) != S_IFREG) || (access(path, X_OK) != 0)) {
            continue;
        } else {
            count++;
        }
    }
    closedir(d);
    printf("%lld\n", count);

    return 0;
}
