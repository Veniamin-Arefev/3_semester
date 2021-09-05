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

int proc(void)
{
    int pid = fork();
    if (!pid) {
        write(1, "1\n", 2);
    }
    return pid;
}

int
main(int argc, char const *argv[]) {
    proc()|proc()|proc();
    return 0;
}
