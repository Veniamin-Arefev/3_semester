#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

struct dirent *
next(char *name, struct dirent *prev_dd)
{
    DIR *d = opendir(name);
    struct dirent *dd;
    struct dirent *res_dd = prev_dd;
    while ((dd = readdir(d))) {
        if (!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, "..")) {
            continue;
        }
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", name, dd->d_name);
        struct stat stb;
        if (stat(path, &stb) < 0) {
            continue;
        }
        if (!S_ISDIR(stb.st_mode)) {
            continue;
        }

        if (!prev_dd) {
            if (res_dd == prev_dd) {
                res_dd = dd;
            } else if (strcasecmp(dd->d_name, res_dd->d_name) < 0) {
                res_dd = dd;
            }
        } else {
            if (strcasecmp(dd->d_name, prev_dd->d_name) > 0) {
                if (res_dd == prev_dd) {
                    res_dd = dd;
                } else if (strcasecmp(dd->d_name, res_dd->d_name) < 0) {
                    res_dd = dd;
                }
            }
        }
    }
    closedir(d);
    if (res_dd != prev_dd) {
        return res_dd;
    }
    return NULL;
}

void recur_tree(char *name)
{
    struct dirent *dd = NULL;
    while ((dd = next(name, dd))) {
        char new_name[PATH_MAX];
        snprintf(new_name, sizeof(new_name), "%s/%s", name, dd->d_name);
        DIR *d = opendir(new_name);
        if (!d) {
            continue;
        }
        closedir(d);
        if((strlen(dd->d_name) + strlen(name)) > (PATH_MAX - 1)) {
            continue;
        }
        printf("cd %s\n", dd->d_name);
        recur_tree(new_name);
        printf("cd ..\n");
    }
}

int
main(int argc, char **argv)
{
    recur_tree(argv[1]);

    return 0;
}
