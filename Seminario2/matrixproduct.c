#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void init(double *M, int n) {
    for (int i = 0; i < n*n; i++) M[i] = (double)rand() / RAND_MAX;
}

void zero(double *M, int n) {
    for (int i = 0; i < n*n; i++) M[i] = 0.0;
}

int equal(double *A, double *B, int n) {
    for (int i = 0; i < n*n; i++)
        if (fabs(A[i] - B[i]) > 1e-9) return 0;
    return 1;
}

void ijk(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void ikj(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void jik(double *A, double *B, double *C, int n) {
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void jki(double *A, double *B, double *C, int n) {
    for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void kij(double *A, double *B, double *C, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

void kji(double *A, double *B, double *C, int n) {
    for (int k = 0; k < n; k++)
        for (int j = 0; j < n; j++)
            for (int i = 0; i < n; i++)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    if (n <= 0) return 1;

    double *A = malloc(n*n*sizeof(double));
    double *B = malloc(n*n*sizeof(double));
    double *C = malloc(n*n*sizeof(double));
    double *Ref = malloc(n*n*sizeof(double));

    srand(42);
    init(A, n); init(B, n);

    zero(Ref, n);
    double t = omp_get_wtime();
    ijk(A, B, Ref, n);
    printf("ijk: %.6f\n", omp_get_wtime() - t);

    void (*vars[5])(double*,double*,double*,int) = {ikj, jik, jki, kij, kji};
    const char *names[5] = {"ikj","jik","jki","kij","kji"};

    for (int v = 0; v < 5; v++) {
        zero(C, n);
        t = omp_get_wtime();
        vars[v](A, B, C, n);
        printf("%s: %.6f %s\n", names[v], omp_get_wtime() - t, equal(Ref, C, n) ? "OK" : "FAIL");
    }

    free(A); free(B); free(C); free(Ref);
    return 0;
}