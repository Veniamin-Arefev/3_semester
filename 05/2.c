#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int
main(int argc, char const *argv[])
{
    const char s[] = "rwxrwxrwx";
    for (int i = 1; i < argc; i ++) {
        unsigned long long len = strlen(argv[i]);
        unsigned long long mask = 0;
        for (int j = 0; j < len; j++) {
            char ch = argv[i][j] - '0';
            if (argv[i][j] != 0) {
                mask = (mask << 3) | ch;
            }
        }
        unsigned long long mymask = 0400;
        int iter = 0;
        while (mymask) {
            if (mymask & mask) {
                printf("%c", s[iter]);
            } else {
                printf("%c", '-');
            }
            mymask >>= 1;
            iter++;
        }
        printf("\n");
    }
    return 0;
}
