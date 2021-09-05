#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char const *argv[]) {
    unsigned int maxver, maxhor;
    sscanf(argv[2], "%u", &maxver);
    sscanf(argv[3], "%u", &maxhor);

//    int fd = open(argv[1], O_RDONLY, 0);
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        printf("Cant create file\n");
        return 1;
    }

    size_t pagesize = getpagesize();

    size_t size = (((maxver * maxhor * sizeof(int32_t) - 1) / pagesize) + 1) * pagesize;
    if (ftruncate(fd, size) == -1) {
        printf("Failed to resize file\n");
        return 1;
    }

    void *fpt = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (fpt == MAP_FAILED) {
        printf("Cant map file\n");
        return 1;
    }


    int32_t *intpt = (int32_t *) fpt;
    int curhor = 0, curver = 0, current = 1;
    int maxcurrent = maxhor * maxver;
    while (current != maxcurrent) {
        while (curhor < maxhor - 1 - curver) {
            intpt[(curver) * maxhor + curhor++] = current++;
        }
        if (current == maxcurrent) break;
        while (curver < maxver - (maxhor - curhor)) {
            intpt[(curver++) * maxhor + curhor] = current++;
        }
        if (current == maxcurrent) break;
        while (curhor > maxver - 1 - curver) {
            intpt[curver * maxhor + curhor--] = current++;
        }
        if (current == maxcurrent) break;
        while (curver > curhor + 1) {
            intpt[(curver--) * maxhor + curhor] = current++;
        }
    }
    intpt[curver * maxhor + curhor] = current;


    munmap(fpt, size);
    ftruncate(fd, maxver * maxhor * sizeof(int32_t));
    close(fd);

    return 0;
}


















