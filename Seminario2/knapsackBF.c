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
    int best_val = 0, best_wt = 0;

    double t = omp_get_wtime();

    for (long long m = 0; m < total; m++) {
        int cw = 0, cv = 0;
        for (int i = 0; i < n; i++)
            if (m & (1LL << i)) { cw += w[i]; cv += v[i]; }
        if (cw <= cap && cv > best_val) {
            best_val = cv; best_wt = cw;
        }
    }

    printf("seq: %.6f val=%d wt=%d\n", omp_get_wtime() - t, best_val, best_wt);

    free(w); free(v);
    return 0;
}