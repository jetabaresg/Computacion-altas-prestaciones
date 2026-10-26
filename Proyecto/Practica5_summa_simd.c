#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <immintrin.h> // Cabecera para intrinsics SIMD (AVX / FMA)

// Parámetros de bloqueo para la memoria caché (Práctica 4)
#define MC 256
#define NC 256
#define KC 128

// Parámetros del micro-kernel SIMD (4 dobles por registro YMM de 256 bits)
#define MR 4
#define NR 4

// Micro-kernel SIMD 4x4 optimizado mediante registros AVX y FMA
inline void microkernel_simd_4x4(double *Ac, double *Bc, double *C, int ldc, int kc, int ic, int jc) {
    // Cargar 4x4 elementos de C en 4 registros vectoriales YMM
    __m256d c0 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 0) * ldc]);
    __m256d c1 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 1) * ldc]);
    __m256d c2 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 2) * ldc]);
    __m256d c3 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 3) * ldc]);

    for (int k = 0; k < kc; k++) {
        // Cargar vector de 4 elementos de Ac (columna mr=4)
        __m256d a = _mm256_loadu_pd(&Ac[k * MR]);

        // Duplicar el escalar correspondiente de Bc en cada registro vectorial
        __m256d b0 = _mm256_set1_pd(Bc[k + 0 * kc]);
        __m256d b1 = _mm256_set1_pd(Bc[k + 1 * kc]);
        __m256d b2 = _mm256_set1_pd(Bc[k + 2 * kc]);
        __m256d b3 = _mm256_set1_pd(Bc[k + 3 * kc]);

        // Operaciones FMA vectoriales: C_i += A_k * B_k
        c0 = _mm256_fmadd_pd(a, b0, c0);
        c1 = _mm256_fmadd_pd(a, b1, c1);
        c2 = _mm256_fmadd_pd(a, b2, c2);
        c3 = _mm256_fmadd_pd(a, b3, c3);
    }

    // Guardar los 4x4 resultados de vuelta a la matriz C local
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 0) * ldc], c0);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 1) * ldc], c1);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 2) * ldc], c2);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 3) * ldc], c3);
}

// Multiplicación local a bloques que invoca al micro-kernel SIMD
void matmul_local_blocking_simd(double *A, double *B, double *C, int b) {
    for (int jc = 0; jc < b; jc += NC) {
        int nc_curr = (jc + NC > b) ? (b - jc) : NC;

        for (int pc = 0; pc < b; pc += KC) {
            int kc_curr = (pc + KC > b) ? (b - pc) : KC;

            // Empaquetamiento de Bc
            double *Bc = (double *)malloc(kc_curr * nc_curr * sizeof(double));
            for (int j = 0; j < nc_curr; j++) {
                for (int k = 0; k < kc_curr; k++) {
                    Bc[k + j * kc_curr] = B[(pc + k) + (jc + j) * b];
                }
            }

            for (int ic = 0; ic < b; ic += MC) {
                int mc_curr = (ic + MC > b) ? (b - ic) : MC;

                // Empaquetamiento de Ac
                double *Ac = (double *)malloc(mc_curr * kc_curr * sizeof(double));
                for (int k = 0; k < kc_curr; k++) {
                    for (int i = 0; i < mc_curr; i++) {
                        Ac[i + k * mc_curr] = A[(ic + i) + (pc + k) * b];
                    }
                }

                // Bucles paralelos OpenMP sobre sub-bloques jr e ir invocando el micro-kernel
                #pragma omp parallel for collapse(2) default(shared)
                for (int jr = 0; jr < nc_curr; jr += NR) {
                    for (int ir = 0; ir < mc_curr; ir += MR) {
                        microkernel_simd_4x4(&Ac[ir], &Bc[jr * kc_curr], C, b, kc_curr, ic + ir, jc + jr);
                    }
                }

                free(Ac);
            }
            free(Bc);
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = atoi(argv[13]);
    int q = (int)sqrt(size);
    int b = n / q;

    // Crear topología cartesiana 2D SUMMA
    MPI_Comm comm_2d, comm_row, comm_col;
    int dims[14] = {q, q}, periods[14] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);

    int coords[14];
    MPI_Cart_coords(comm_2d, rank, 2, coords);
    int free_row[14] = {0, 1}, free_col[14] = {1, 0};
    MPI_Cart_sub(comm_2d, free_row, &comm_row);
    MPI_Cart_sub(comm_2d, free_col, &comm_col);

    // Reservar matrices locales
    double *A_loc  = (double *)calloc(b * b, sizeof(double));
    double *B_loc  = (double *)calloc(b * b, sizeof(double));
    double *C_loc  = (double *)calloc(b * b, sizeof(double));
    double *A_work = (double *)malloc(b * b * sizeof(double));
    double *B_work = (double *)malloc(b * b * sizeof(double));

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    // Bucle iterativo de SUMMA
    for (int k = 0; k < q; k++) {
        if (coords[13] == k) for (int i = 0; i < b * b; i++) A_work[i] = A_loc[i];
        if (coords == k) for (int i = 0; i < b * b; i++) B_work[i] = B_loc[i];

        MPI_Bcast(A_work, b * b, MPI_DOUBLE, k, comm_row);
        MPI_Bcast(B_work, b * b, MPI_DOUBLE, k, comm_col);

        // Cálculo local con micro-kernel SIMD
        matmul_local_blocking_simd(A_work, B_work, C_loc, b);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();

    if (rank == 0) {
        double t_par = t_end - t_start;
        double gflops = (2.0 * n * n * n) / (t_par * 1e9);
        printf("P5 (SUMMA + Blocking + SIMD) - N=%d - Tiempo: %.4f s - GFLOPS: %.2f\n", n, t_par, gflops);
    }

    free(A_loc); free(B_loc); free(C_loc);
    free(A_work); free(B_work);
    MPI_Finalize();
    return 0;
}