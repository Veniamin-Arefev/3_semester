#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

extern unsigned long long hash_function(unsigned long long);

enum {
    SIZE_OF_ENTRY = sizeof(unsigned long long)
};

int
main(int argc, char const *argv[]) {

    int fd = open(argv[1], O_WRONLY | O_CREAT, 0660);

    unsigned long long currenth;
    sscanf(argv[2], "%llx", &currenth);
    long long count;
    sscanf(argv[3], "%lld", &count);

    unsigned char buf[SIZE_OF_ENTRY];

    lseek(fd, 0, SEEK_END);
    lseek(fd, (int) (count - 1) * (int) sizeof(unsigned long long), SEEK_CUR);

    unsigned long long *address = (unsigned long long *) buf;
    *address = currenth;

    ssize_t countwritten = 0;
    ssize_t bufcount = 0;

    for (int i = 0; i < count; i++) {
        while ((countwritten = write(fd, buf + bufcount, SIZE_OF_ENTRY - bufcount)) + bufcount < SIZE_OF_ENTRY) {
            bufcount += countwritten;
        }
        bufcount = 0;
        currenth = hash_function(currenth);
        *address = currenth;
        lseek(fd, -2 * (int) SIZE_OF_ENTRY, SEEK_CUR);
    }

    close(fd);
    return 0;
}
