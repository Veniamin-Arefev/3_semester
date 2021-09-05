#include <stdio.h>
#include <malloc.h>

int main() {
    int N;
    scanf("%d", &N);

    int *first = malloc(sizeof(int) * (N - 1));

    for (int a = 1; a < N; a++) {
        for (int b = 0; b < N; b++) {
            if (((a * b) % N) == 1) {
                first[a - 1] = b;
            }
        }
    }

    for (int c = 0; c < N; c++) {
        for (int a = 0; a < N - 1; a++) {
            printf("%d ", (c * first[a]) % N);
        }
        printf("\n");
    }

    free(first);

    return 0;
}
