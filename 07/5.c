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

unsigned long long ssize = 0;

void myreaddir(char *abspath, char *relpath, int deep) {
    if (deep > 4) {
        return;
    }
    DIR *dir = opendir(abspath);
    if (!dir) {
        return;
    }
    struct dirent *dr;
    while ((dr = readdir(dir))) {
        if (strcmp(dr->d_name, ".") == 0 || strcmp(dr->d_name, "..") == 0) {
            continue;
        }
        char newabspath[PATH_MAX + 1];
        snprintf(newabspath, sizeof(newabspath), "%s/%s", abspath, dr->d_name);
        struct stat stb;
        if (lstat(newabspath, &stb) < 0) {
            continue;
        }

        char pathrel[PATH_MAX + 1];
        if (strlen(relpath) != 0) {
            sprintf(pathrel, "%s/%s", relpath, dr->d_name);
        } else {
            sprintf(pathrel, "%s", dr->d_name);
        }

        if ((stb.st_mode & S_IFMT) == S_IFREG) {
            if (access(newabspath, R_OK) == 0) {
                printf("%s\n", pathrel);
                if (stb.st_size <= ssize) {
                    printf("%s\n", pathrel);
                }
            }
        }
        if ((stb.st_mode & S_IFMT) == S_IFDIR) {
            myreaddir(newabspath, pathrel, deep + 1);
        }
    }
}


int
main(int argc, char const *argv[]) {

    sscanf("%lld", argv[2], &ssize);
    char pathabs[PATH_MAX + 1];
    char pathrel[PATH_MAX + 1];
    memset(pathabs, 0,sizeof(pathabs));
    memset(pathrel, 0,sizeof(pathrel));
    sprintf(pathabs, "%s", argv[1]);
    myreaddir(pathabs, pathrel, 1);

    return 0;
}
