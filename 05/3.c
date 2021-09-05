#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int
main(int argc, char const *argv[]) {
    uint32_t n, s, w;
    scanf("%u %u %u", &n, &s, &w);
    char *wstr = malloc(15);
    sprintf(wstr, "%d", w);
    char *escape = "|";
    char *percent = "%";
    char *format = malloc(30);
    char *ch = malloc(5);
    format[0] = '\0';
    ch[1] = '\0';
    strcat(format, escape);
    strcat(format, percent);
    strcat(format, wstr);
    ch[0] = 'o';
    strcat(format, ch);
    strcat(format, escape);
    strcat(format, percent);
    strcat(format, wstr);
    ch[0] = 'u';
    strcat(format, ch);
    strcat(format, escape);
    strcat(format, percent);
    strcat(format, wstr);
    ch[0] = 'd';
    strcat(format, ch);
    strcat(format, escape);

    uint32_t nownumber = 0;
    uint32_t prevnumber = 0;

    uint32_t sigmask = 0;
    uint32_t singbit = 1u << (n - 1);

    for (int i = 0; i < n - 1; i++) {
        sigmask = sigmask << 1 | 1;
    }

    uint32_t unsmask = sigmask << 1 | 1;

    while ((nownumber >= prevnumber) && nownumber <= unsmask) {
        prevnumber = nownumber;
        nownumber += s;

        uint32_t unsign = prevnumber & unsmask;
        int32_t signnum;

        if (prevnumber & singbit) {
            signnum = -(prevnumber & sigmask);
        } else {
            signnum = prevnumber & sigmask;
        }

        printf(format, prevnumber, unsign, signnum);
        printf("\n");
    }
    free(wstr);
    free(format);
    free(ch);
    return 0;
}
