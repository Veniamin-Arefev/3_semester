#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <stdio.h>

char *normalize_path(const char *path) {
    char *norm = calloc(strlen(path)+1, sizeof(char));
    int nindex = 0, i = 0;
    while (path[i] != '\0') {
//        printf("%2d %c %2d %s\n", i, path[i], nindex, norm);
        if (path[i] == '.' && path[i + 1] != '\0' && path[i + 1] == '.') {
            if (nindex > 1) {
                norm[--nindex] = '\0';
                nindex--;
//                printf("%2d %s\n", nindex, norm);
                while (norm[nindex] != '/' && nindex >= 1) {
                    norm[nindex] = '\0';
//                    printf("%3d %s\n", nindex, norm);
                    if (nindex == 0) {
                        break;
                    } else {
                        nindex--;
                    }
                }
                norm[nindex] = '\0';
            } else if (nindex == 1) {
                norm[--nindex] = '\0';
            }
            i += 2;
        } else if (path[i] != '.') {
            norm[nindex++] = path[i++];
        } else {
            i++;
        }
    }
    if (norm[nindex] == '\0' && nindex == 0) {
        norm[nindex++] = '/';
    }
    return norm;
}


char *relativize_path(const char *path1, const char *path2) {
    char *mypath1 = normalize_path(path1);
    char *mypath2 = normalize_path(path2);
    char *s = calloc(PATH_MAX, sizeof(char));
    int sindex = 0;
    int i = 0;
    int nameStart1 = strlen(mypath1);
    while (mypath1[nameStart1--] != '/');
    nameStart1 += 1;
    int nameStart2 = strlen(mypath2);
    while (mypath2[nameStart2--] != '/');
    nameStart2 += 1;
    while (mypath1[i] == mypath2[i] && (i < nameStart1)) {
        i++;
    }
    int count = 0;
    int j = 1;
    if (mypath1[i] != '\0') {
        while (mypath1[i + j] != '\0') {
            if (mypath1[i + j] == '/') {
                count++;
            }
            j++;
        }
    }
//    printf("%3d %3d %3d %3d ", i, nameStart1, nameStart2, count);
    for (int k = 0; k < count; k++) {
        s[sindex] = '.';
        s[sindex + 1] = '.';
        s[sindex + 2] = '/';
        sindex += 3;
    }
    if (mypath2[i] == '/') {
        i++;
    }
    while (mypath2[i] != '\0') {
        s[sindex++] = mypath2[i++];
    }
    if (s[0] == '\0') {
        s[0] = '.';
    }
    if (sindex > 0) {
        if (s[sindex - 1] == '/') {
            s[sindex - 1] = '\0';
        }
    }
    free(mypath1);
    free(mypath2);
    return s;
}
