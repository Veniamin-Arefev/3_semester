#include <stdio.h>

int main() {
    int N;
    scanf("%d", &N);

    for (int c = 0; c < N; c++) {
        for (int a = 1; a < N; a++) {
            for (int b = 0; b < N; b++) {
                if (((a * b) % N) == c) {
                    printf("%d ", b);
                }
            }
        }
        printf("\n");
    }

    return 0;
}
