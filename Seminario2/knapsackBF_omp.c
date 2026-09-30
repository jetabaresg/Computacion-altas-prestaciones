#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    if (n <= 0 || n > 30) return 1;

    int cap = n * 10;
    int *w = malloc(n * sizeof(int));
    int *v = malloc(n * sizeof(int));

    srand(42);
    for (int i = 0; i < n; i++) {
        w[i] = rand() % 20 + 1;
        v[i] = rand() % 50 + 1;
    }

    long long total = 1LL << n;
    long long best_key = 0;  // (val << 32) | wt

    double t = omp_get_wtime();

    #pragma omp parallel for schedule(static) reduction(max:best_key)
    for (long long m = 0; m < total; m++) {
        int cw = 0, cv = 0;
        for (int i = 0; i < n; i++)
            if (m & (1LL << i)) { cw += w[i]; cv += v[i]; }

        if (cw <= cap) {
            long long key = ((long long)cv << 32) | (unsigned int)cw;
            if (key > best_key) best_key = key;
        }
    }

    int best_val = (int)(best_key >> 32);
    int best_wt = (int)(best_key & 0xFFFFFFFF);

    printf("omp: %.6f val=%d wt=%d\n", omp_get_wtime() - t, best_val, best_wt);

    free(w); free(v);
    return 0;
}