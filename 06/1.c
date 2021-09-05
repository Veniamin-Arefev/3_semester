#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int
main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);
    n -= 1900;
    struct tm *ptime = malloc(sizeof(struct tm));
    memset(ptime, 0, sizeof(struct tm));
    ptime->tm_year = n;
    ptime->tm_mday = 1;
    ptime->tm_isdst = -1;
    mktime(ptime);
    if (ptime->tm_wday > 4) {
        ptime->tm_mday += 11 - ptime->tm_wday;
    } else {
        ptime->tm_mday += 4 - ptime->tm_wday;
    }
    ptime->tm_isdst = -1;
    mktime(ptime);
    int count = 1;
    int lastmon = 0;
    while (ptime->tm_year == n) {
        ptime->tm_isdst = -1;
        ptime->tm_mday += 7;
        count++;
        mktime(ptime);
        if (lastmon != ptime->tm_mon) {
            count = 1;
            lastmon = ptime->tm_mon;
        }
        if ((count % 2 == 0) && (ptime->tm_mday % 3 != 0)) {
            printf("%d %d\n", ptime->tm_mon+1, ptime->tm_mday);
        }
    }
    free(ptime);
    return 0;
}
