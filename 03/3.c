#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[]) {
    double money;
    double temp;
    sscanf(argv[1], "%lf", &money);
    for (int i = 2; i < argc; i++) {
        sscanf(argv[i], "%lf", &temp);
        temp/=100;
        temp+=1.0;
        money*=temp;
        money = round(money * 10000) / 10000;
    }
    printf("%.4lf\n", money);
    return 0;
}
