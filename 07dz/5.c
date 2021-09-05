#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>

struct List
{
    char * dirname;
    char * dirpath;
    struct List *next;
};

struct List *add(struct List *head, char *dirpath, char *dirname) {
    struct List *newlist = malloc(sizeof(struct List));
    newlist->dirpath = dirpath;
    newlist->dirname = dirname;

    if (head == NULL) {
        newlist->next = NULL;
        return  newlist;
    }
    struct List *predelem = NULL;
    struct List *curelem = head;
    while ((curelem != NULL) && (strcasecmp(curelem->dirpath, dirpath) < 0)) {
        predelem = curelem;
        curelem = curelem->next;
    }
    if (predelem == NULL) {
        newlist->next = curelem;
        return newlist;
    } else {
        newlist->next = curelem;
        predelem->next = newlist;
        return head;
    }
}

void myreaddir(char *abspath) {

    DIR *dir = opendir(abspath);
    if (!dir) {
        return;
    }

    struct List *head = NULL;

    struct dirent *dr;
    while ((dr = readdir(dir))) {
        if (strcmp(dr->d_name, ".") == 0 || strcmp(dr->d_name, "..") == 0) {
            continue;
        }

        if (strlen(abspath) + strlen(dr->d_name) > PATH_MAX) {
            continue;
        }

        char *newabspath = calloc(PATH_MAX + 1, sizeof(char));
        snprintf(newabspath, PATH_MAX + 1, "%s/%s", abspath, dr->d_name);


        struct stat stb;
        if (lstat(newabspath, &stb) < 0) {
            continue;
        }

        if ((stb.st_mode & S_IFMT) == S_IFDIR) {
            char *dirname = malloc(NAME_MAX);
            strcpy(dirname, dr->d_name);
            head = add(head, newabspath, dirname);
        } else {
            free(newabspath);
        }
    }
    closedir(dir);

    while (head != NULL) {
        DIR *dir1 = opendir(head->dirpath);
        if (dir1 != NULL) {
            closedir(dir1);
            printf("cd %s\n", head->dirname);
            myreaddir(head->dirpath);
            printf("cd ..\n");
        }
        free(head->dirpath);
        free(head->dirname);
        struct List *oldhead = head;
        head = head->next;
        free(oldhead);
    }
}

int
main(int argc, char const *argv[]) {

    char pathabs[PATH_MAX + 1];
    memset(pathabs, 0, sizeof(pathabs));
    sprintf(pathabs, "%s", argv[1]);
    myreaddir(pathabs);

    return 0;
}

