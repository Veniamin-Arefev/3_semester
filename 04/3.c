#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>

enum
{
    SIZE_OF_ENTRY = 12,
    SECOND_FIELD = 4,
    THIRD_FIELD = 8,
    FIRST_BIT_SHIFT = 24,
    SECOND_BIT_SHIFT = 16,
    THIRD_BIT_SHIFT = 8
};

int fd;

int32_t getBigEndian(const unsigned char *buf) {
    int32_t temp1 =
            ((unsigned int) buf[3]) << FIRST_BIT_SHIFT |
            ((unsigned int) buf[2]) << SECOND_BIT_SHIFT |
            ((unsigned int) buf[1]) << THIRD_BIT_SHIFT |
            ((unsigned int) buf[0]);
    return temp1;
}

void
treebypass(int32_t index)
{
    lseek(fd, SIZE_OF_ENTRY * index, SEEK_SET);
    unsigned char buf[SIZE_OF_ENTRY];
    ssize_t count = 0;
    ssize_t bufcount = 0;
    while ((count = read(fd, buf + bufcount, SIZE_OF_ENTRY - bufcount)) > 0) {
        if (bufcount + count < SIZE_OF_ENTRY) {
            bufcount += count;
            continue;
        } else {
            bufcount = 0;
            int32_t key = getBigEndian(buf);
            if (key == 0) {
            } else {
                int32_t temp = getBigEndian(buf + THIRD_FIELD);
                if (temp != 0) {
                    treebypass(getBigEndian(buf + THIRD_FIELD));
                }
                temp = getBigEndian(buf + SECOND_FIELD);
                printf("%d\n", key);
                if (temp != 0) {
                    treebypass(getBigEndian(buf + SECOND_FIELD));
                }
            }
            break;
        }
    }
}

int
main(int argc, char const *argv[])
{
    fd = open(argv[1], O_RDONLY);
    treebypass(0);
    close(fd);
    return 0;
}
