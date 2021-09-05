#include <stdio.h>

int
main(int argc, char const *argv[])
{
    long long pos = 0;
    long long neg = 0;

    int temp;
    for (int i = 1; i < argc; i++) {
        sscanf(argv[i], "%d", &temp);
        if (temp > 0) {
            pos += temp;
        } else {
            neg += temp;
        }
    }
    printf("%lld\n%lld\n", pos, neg);
    return 0;
}
