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

int
main(int argc, char const *argv[]) {

    int fd = open(argv[1], O_RDWR);
    off_t first, second;
    off_t end = lseek(fd, 0, SEEK_END);

    off_t cur = lseek(fd, 0, SEEK_SET);
    while (cur != end) {
        off_t min_index = cur;
        int min;
        int number;
        read(fd, &min, sizeof(number));
        while (read(fd, &number, sizeof(number)) > 0) {
            if (number <= min) {
                min = number;
                min_index = lseek(fd, 0, SEEK_CUR);
                min_index -= sizeof(int);
            }
        }
        if (min_index != cur) { //rewrite
            lseek(fd, cur, SEEK_SET);
            read(fd, &number, sizeof(int));
            lseek(fd, cur, SEEK_SET);
            write(fd, &min, sizeof(int));
            lseek(fd, min_index, SEEK_SET);
            write(fd, &number, sizeof(int));
        }

        cur += (off_t) sizeof(int);
        lseek(fd, cur, SEEK_SET);
    }


    return 0;
}
