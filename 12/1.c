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
#include <wait.h>
#include <signal.h>

STYPE bit_reverse(STYPE value) {
    UTYPE input = value;
    UTYPE retval = 0;
    UTYPE number = 0-1;
    while (number > 0) {
        UTYPE last_bit = input & 1u;
        retval = retval << 1;
        retval = retval | last_bit;
        input = input >> 1;
        number = number >> 1;
    }

    return (STYPE)retval;
}