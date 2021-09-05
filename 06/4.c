#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum
{
    HOUR = 12,
    START_DAY = 7,
    START_MON = 9,
    START_YEAR = 25,
    IS_DAY_SAVING_TIME = -1,
    DAY_SECONDS = 60 * 60 * 24,
    MON_DAYS = 30,
    YEAR_MONTHS = 12
};


int
main(int argc, char const *argv[]) {
    struct tm startTime;
    memset(&startTime, 0, sizeof(struct tm));
//    startTime.tm_hour = HOUR;
    startTime.tm_mday = START_DAY;
    startTime.tm_mon = START_MON;
    startTime.tm_year = START_YEAR;
    startTime.tm_isdst = IS_DAY_SAVING_TIME;
    time_t timeoffset = mktime(&startTime);
    int y, m, d;
    while (scanf("%d %d %d", &y, &m, &d) > 0) {
        y -= 1900;
        struct tm ptime;
        memset(&ptime, 0, sizeof(struct tm));
        ptime.tm_year = y;
        ptime.tm_mon = m - 1;
        ptime.tm_mday = d;
        ptime.tm_hour = HOUR;
        ptime.tm_isdst = IS_DAY_SAVING_TIME;
        unsigned long long timecur = mktime(&ptime);
        timecur -= timeoffset;
        unsigned long long curdays = timecur / DAY_SECONDS;
        unsigned long long curmon = curdays / MON_DAYS;
        unsigned long long curyear = curmon / YEAR_MONTHS;
        printf("%lld %lld %lld\n", curyear + 1, curmon % YEAR_MONTHS + 1, curdays % MON_DAYS + 1);
    }
    return 0;
}
