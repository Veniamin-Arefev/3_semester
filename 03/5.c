#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum { NUMBER_OF_DIGITS = 10 };

int
main(int argc, char const *argv[])
{
    unsigned long long *mas = malloc(NUMBER_OF_DIGITS * sizeof(long long));
    memset(mas, 0, sizeof(long long ));

    char name_buf[PATH_MAX + 16];
    fgets(name_buf, sizeof(name_buf), stdin);

    if (strlen(name_buf) != 0) {
        unsigned long len = strlen(name_buf);
        if (name_buf[len - 1] == '\n') {
            name_buf[len - 1] = '\0';
            if (name_buf[len - 2] == '\r') {
                name_buf[len - 2] = '\0';
            }
        } else {
            if (name_buf[len - 1] == '\r') {
                name_buf[len - 1] = '\0';
            }
        }
        FILE *filein = fopen(name_buf, "r");
        if (filein != NULL) {
            int inchar;
            while ((inchar = getc_unlocked(filein)) != EOF) {
                if (inchar >= '0' && inchar <= '9') {
                    mas[inchar - '0'] += 1;
                }
            }
            fclose(filein);
        }
    }
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        printf("%d %llu\n", i, mas[i]);
    }
    free(mas);
    return 0;
}
