#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

enum { SIZE_OF_ENTRY = sizeof(unsigned long long) };

int
main(int argc, char const *argv[]) {
    int fd = open(argv[1], O_RDWR);

    if (fd) {
        unsigned char buf[SIZE_OF_ENTRY];
        ssize_t count = 0;
        ssize_t bufcount = 0;
        signed long long min = LLONG_MAX;
        int readedonce = 0;
        off_t minoffset = 0;
        while ((count = read(fd, buf + bufcount, SIZE_OF_ENTRY - bufcount)) > 0) {
            if (bufcount + count < SIZE_OF_ENTRY) {
                bufcount += count;
                continue;
            } else {
                if (readedonce) {
                    signed long long temp = *((signed long long *) buf);
                    if (temp < min) {
                        minoffset = lseek(fd, 0, SEEK_CUR);
                        minoffset -= (int) sizeof(long long);
                        min = temp;
                    }
                } else {
                    readedonce = 1;
                    signed long long temp = *((signed long long *) buf);
                    minoffset = lseek(fd, 0, SEEK_CUR);
                    minoffset -= (int) sizeof(long long);
                    min = temp;
                }
                bufcount = 0;
            }
        }

        if (readedonce) {
            lseek(fd, minoffset, SEEK_SET);
            unsigned long long *address = (unsigned long long *) buf;
            *address = -((unsigned long long) min);
            while ((count = write(fd, buf + bufcount, SIZE_OF_ENTRY - bufcount)) > 0) {
                if (bufcount + count < SIZE_OF_ENTRY) {
                    bufcount += count;
                } else {
                    break;
                }

            }
        }
        close(fd);
    }
    return 0;
}
