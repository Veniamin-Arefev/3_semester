#include <stdio.h>
#include <malloc.h>

int main() {
    int N;
    scanf("%d", &N);

    for (int c = 0; c < N; c++) {
        for (int a = 1; a < N; a++) {
            printf("%d ", (c*a)%N);
        }
        printf("\n");
    }
    return 0;
}
