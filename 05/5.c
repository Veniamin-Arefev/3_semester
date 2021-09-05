#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
    SIZE_OF_ENTRY = sizeof(uint16_t),
    FIRST_BIT_SHIFT = 8
};

int
main(int argc, char const *argv[]) {

    int fd = open(argv[1], O_RDONLY);

    if (fd != -1) {
        unsigned char buf[SIZE_OF_ENTRY];
        ssize_t count = 0;
        ssize_t bufcount = 0;
        uint32_t min = 0;
        int readedonce = 0;
        while ((count = read(fd, buf + bufcount, SIZE_OF_ENTRY - bufcount)) > 0) {
            if (bufcount + count < SIZE_OF_ENTRY) {
                bufcount += count;
                continue;
            } else {
                if (buf[SIZE_OF_ENTRY-1] % 2 == 0) {
                    if (readedonce) {
                        uint32_t temp = buf[0] << FIRST_BIT_SHIFT | buf[1];
                        if (temp < min) {
                            min = temp;
                        }
                    } else {
                        readedonce = 1;
                        min = buf[0] << FIRST_BIT_SHIFT | buf[1];
                    }
                }
                bufcount = 0;
            }
        }
        if (readedonce) {
            printf("%d\n", min);
        }

        close(fd);
    }
    return 0;
}
