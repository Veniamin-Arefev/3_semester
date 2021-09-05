#include <stdio.h>
#include <string.h>
#include <ctype.h>

int mystrspccmp(const char *str1, const char *str2) {
    const unsigned char *s1 = (const unsigned char *) str1;
    const unsigned char *s2 = (const unsigned char *) str2;

    for (;;) {
        //printf("%d %d \n", *s1, *s2);
        for (;;) {
            if (!isspace((*s1))) {
                break;
            }
            s1++;
        }
        for (;;) {
            if (!isspace((*s2))) {
                break;
            }
            s2++;
        }
        //printf("%d %d \n", *s1, *s2);

        if ((*s1) == (*s2) && (*s1) == '\0') {
            return 0;
        }

        if ((*s1) == (*s2)) {
            s1++;
            s2++;
            continue;
        }

        if ((*s1) == '\0') {
            return -1;
        }

        if ((*s2) == '\0') {
            return 1;
        }

        if ((*s1) > (*s2)) {
            return 1;
        }

        if ((*s1) < (*s2)) {
            return -1;
        }
    }
}

int main(int argc, char const *argv[]) {
    char str1[100] = "a         a                  a          a";
    char str2[100] = "a                        a           a                      a";
    printf("%d\n", mystrspccmp(str1, str2));
    //printf("%d\n", "а");
    return 0;
}