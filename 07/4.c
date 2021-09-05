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

int
main(int argc, char const *argv[]) {
    struct tm time;
    memset(&time, 0 , sizeof(struct tm));
    int startyear, startmon, startday, duryear;
    if (scanf("%d-%d-%d %d", &startyear, &startmon, &startday, &duryear) != 4) {
        return 0;
    }
    time.tm_year = startyear-1900;
    time.tm_mon = startmon-1;
    time.tm_mday = startday;

    mktime(&time);
    if (duryear > 0) {
        for (int i = 0; i < 2 * duryear; i++) {
            time.tm_mon += 6;
            mktime(&time);
            if (time.tm_wday == 0) {
                time.tm_mday += 1;
                mktime(&time);
                printf("%04d-%02d-%02d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
                time.tm_mday -= 1;
            } else if (time.tm_wday == 6) {
                time.tm_mday += 2;
                mktime(&time);
                printf("%04d-%02d-%02d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
                time.tm_mday -= 2;
            } else {
                printf("%04d-%02d-%02d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
            }
        }
    }

    return 0;
}
