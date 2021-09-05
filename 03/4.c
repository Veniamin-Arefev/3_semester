#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

enum
{
    BUFFER_SIZE = 16
};

int
main(int argc, char const *argv[])
{
    char *buf = malloc(BUFFER_SIZE);
    int wasminus = 0;
    int produced = 1;
    ssize_t count;
    unsigned long long answer = 0;
    unsigned long long temp = 0;
    while ((count = read(0, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < count; i++) {
            if (!isspace(buf[i])) {
                if (buf[i] == '-') {
                    wasminus = 1;
                    continue;
                }
                if (buf[i] == '+') {
                    continue;
                }
                temp *= 10;
                temp += buf[i] - '0';
                produced = 0;
            } else {
                if (produced) {
                    continue;
                }
                if (wasminus) {
                    answer -= temp;
                } else {
                    answer += temp;
                }
                produced = 1;
                temp = 0;
                wasminus = 0;
            }
        }
    }
    printf("%lld\n", answer);
    return 0;
}
