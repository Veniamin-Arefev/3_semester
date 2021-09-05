#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

enum
{
    ENTRY_LENGTH = 20,
    NUMBER_START = 16,
    FIRST_BIT_SHIFT = 24,
    SECOND_BIT_SHIFT = 16,
    THIRD_BIT_SHIFT = 8
};


int
main(int argc, char const *argv[])
{
    signed long long first = -LLONG_MAX;
    signed long long second = -LLONG_MAX;
    int firstread = 0;
    int secondread = 0;
    for (int i = 1; i < argc; ++i) {
        int fdesc = open(argv[i], O_RDONLY);
        unsigned char buf[ENTRY_LENGTH];
        ssize_t count = 0;
        ssize_t bufcount = 0;
        while ((count = read(fdesc, buf + bufcount, ENTRY_LENGTH - bufcount)) > 0) {
            if (bufcount + count < ENTRY_LENGTH) {
                bufcount += count;
                continue;
            } else {
                bufcount = 0;
                signed int temp1 =
                        ((unsigned int) buf[NUMBER_START + 3]) << FIRST_BIT_SHIFT |
                        ((unsigned int) buf[NUMBER_START + 2]) << SECOND_BIT_SHIFT |
                        ((unsigned int) buf[NUMBER_START + 1]) << THIRD_BIT_SHIFT |
                        ((unsigned int) buf[NUMBER_START + 0]);
                signed long long temp = temp1;
                if (!firstread) {
                    first = temp;
                    firstread = 1;
                } else {
                    if (!secondread) {
                        if (first == temp) {
                            continue;
                        }
                        if (first < temp) {
                            second = first;
                            first = temp;
                        } else {
                            second = temp;
                        }
                        secondread = 1;
                    } else {
                        if (first == temp || second == temp) {
                            continue;
                        }
                        if (temp > first) {
                            second = first;
                            first = temp;
                        } else if (temp > second) {
                            second = temp;
                        }
                    }
                }
            }
        }
        close(fdesc);
    }
    if (secondread) {
        if (second < 0 && second > -100) {
            printf("-%lld.%02lld\n", second / 100, llabs(second % 100));
        } else {
            printf("%lld.%02lld\n", second / 100, llabs(second % 100));
        }
    }
    return 0;
}
