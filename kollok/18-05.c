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

int GetBit(unsigned char *ptr, unsigned bit_number) {
    unsigned index = bit_number >> 3;
    return (ptr[index] >> (bit_number & 07)) & 1;
}

int
main(int argc, char const *argv[]) {

    unsigned char pointer[2];
    pointer[0] = 0x95;

    printf("%d\n", GetBit(pointer, 0));



    return 0;
}
