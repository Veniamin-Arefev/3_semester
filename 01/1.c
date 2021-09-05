#include <stdio.h>

int main() {
    double x, y;
    scanf("%lf %lf", &x, &y);

    if (y >= 2.0 && y <=5.0 && x >= 1 && x <= 7) {
        if (x + y <= 10.0) {
            printf("1");
            return 0;
        }
    }
    printf("0");
    return 0;
}
