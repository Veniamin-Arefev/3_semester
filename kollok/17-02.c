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

int is_available(unsigned char *start, unsigned block_index, unsigned max_block_index) {
    if (block_index >= max_block_index) {
        return -1; //out of bound
    } else {
        return (start[block_index >> 3] >> (block_index & 07)) & 1
    }
}

int
main(int argc, char const *argv[]) {


    return 0;
}
