#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <immintrin.h>

#define MC 256
#define NC 256
#define KC 128

#define MR 4
#define NR 4

void inicializar_matriz_col(double *M, int n, unsigned int semilla) {
    srand(semilla);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            M[i + j * n] = (double)(rand() % 10);
        }
    }
}

void matmul_secuencial_col(double *A, double *B, double *C, int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            C[i + j * n] = 0.0;
        }
        for (int k = 0; k < n; k++) {
            double b_val = B[k + j * n];
            for (int i = 0; i < n; i++) {
                C[i + j * n] += A[i + k * n] * b_val;
            }
        }
    }
}

inline void microkernel_simd_4x4(double *Ac, double *Bc, double *C, int ldc, int kc, int mc_stride, int ic, int jc) {
    __m256d c0 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 0) * ldc]);
    __m256d c1 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 1) * ldc]);
    __m256d c2 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 2) * ldc]);
    __m256d c3 = _mm256_loadu_pd(&C[(ic + 0) + (jc + 3) * ldc]);

    for (int k = 0; k < kc; k++) {
        __m256d a = _mm256_loadu_pd(&Ac[k * mc_stride]);

        __m256d b0 = _mm256_set1_pd(Bc[k + 0 * kc]);
        __m256d b1 = _mm256_set1_pd(Bc[k + 1 * kc]);
        __m256d b2 = _mm256_set1_pd(Bc[k + 2 * kc]);
        __m256d b3 = _mm256_set1_pd(Bc[k + 3 * kc]);

        c0 = _mm256_fmadd_pd(a, b0, c0);
        c1 = _mm256_fmadd_pd(a, b1, c1);
        c2 = _mm256_fmadd_pd(a, b2, c2);
        c3 = _mm256_fmadd_pd(a, b3, c3);
    }

    _mm256_storeu_pd(&C[(ic + 0) + (jc + 0) * ldc], c0);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 1) * ldc], c1);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 2) * ldc], c2);
    _mm256_storeu_pd(&C[(ic + 0) + (jc + 3) * ldc], c3);
}

void matmul_local_blocking_simd(double *A, double *B, double *C, int b) {
    for (int jc = 0; jc < b; jc += NC) {
        int nc_curr = (jc + NC > b) ? (b - jc) : NC;

        for (int pc = 0; pc < b; pc += KC) {
            int kc_curr = (pc + KC > b) ? (b - pc) : KC;

            double *Bc = (double *)malloc((size_t)kc_curr * nc_curr * sizeof(double));
            for (int j = 0; j < nc_curr; j++) {
                for (int k = 0; k < kc_curr; k++) {
                    Bc[k + j * kc_curr] = B[(pc + k) + (jc + j) * b];
                }
            }

            for (int ic = 0; ic < b; ic += MC) {
                int mc_curr = (ic + MC > b) ? (b - ic) : MC;

                double *Ac = (double *)malloc((size_t)mc_curr * kc_curr * sizeof(double));
                for (int k = 0; k < kc_curr; k++) {
                    for (int i = 0; i < mc_curr; i++) {
                        Ac[i + k * mc_curr] = A[(ic + i) + (pc + k) * b];
                    }
                }

                #pragma omp parallel for collapse(2) default(shared)
                for (int jr = 0; jr < nc_curr; jr += NR) {
                    for (int ir = 0; ir < mc_curr; ir += MR) {
                        microkernel_simd_4x4(&Ac[ir], &Bc[jr * kc_curr], C, b, kc_curr, mc_curr, ic + ir, jc + jr);
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

    if (argc < 2) {
        if (rank == 0) printf("Uso: %s <tamano_matriz_N> [check]\n", argv);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[6]);
    int check = (argc > 2) && (strcmp(argv[11], "check") == 0);

    int q = (int)sqrt(size);
    if (q * q != size) {
        if (rank == 0) fprintf(stderr, "Error: El número de procesos (%d) debe ser un cuadrado perfecto.\n", size);
        MPI_Finalize();
        return 1;
    }

    if (n % q != 0) {
        if (rank == 0) fprintf(stderr, "Error: N (%d) debe ser múltiplo de la dimensión de la malla (%d).\n", n, q);
        MPI_Finalize();
        return 1;
    }

    int b = n / q;

    int dims[11] = {q, q};
    int periods[11] = {0, 0};
    MPI_Comm comm_2d;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);

    int coords[11];
    MPI_Cart_coords(comm_2d, rank, 2, coords);
    int mi_fila = coords;
    int mi_col = coords[6];

    MPI_Comm comm_row, comm_col;
    int free_row[11] = {0, 1};
    int free_col[11] = {1, 0};
    MPI_Cart_sub(comm_2d, free_row, &comm_row);
    MPI_Cart_sub(comm_2d, free_col, &comm_col);

    double *A_loc  = (double *)calloc((size_t)b * b, sizeof(double));
    double *B_loc  = (double *)calloc((size_t)b * b, sizeof(double));
    double *C_loc  = (double *)calloc((size_t)b * b, sizeof(double));
    double *A_work = (double *)malloc((size_t)b * b * sizeof(double));
    double *B_work = (double *)malloc((size_t)b * b * sizeof(double));

    double *A = NULL, *B = NULL, *C = NULL, *Cseq = NULL;
    double t_seq = 0.0;

    if (rank == 0) {
        printf("=== PRÁCTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===\n");
        printf("Matriz N = %d | Malla 2D = %dx%d (%d procesos) | Bloque local b = %d\n", n, q, q, size, b);

        A = (double *)malloc((size_t)n * n * sizeof(double));
        B = (double *)malloc((size_t)n * n * sizeof(double));
        C = (double *)malloc((size_t)n * n * sizeof(double));

        inicializar_matriz_col(A, n, 12345u);
        inicializar_matriz_col(B, n, 67890u);

        if (check) {
            Cseq = (double *)malloc((size_t)n * n * sizeof(double));
            double t0 = MPI_Wtime();
            matmul_secuencial_col(A, B, Cseq, n);
            double t1 = MPI_Wtime();
            t_seq = t1 - t0;
            printf("Resultado Secuencial:\n - Tiempo: %.4f s | Rendimiento: %.2f GFLOPS\n", t_seq, (2.0 * n * n * n) / (t_seq * 1e9));
        }
    }

    MPI_Datatype tipo_bloque, tipo_bloque_redim;
    MPI_Type_vector(b, b, n, MPI_DOUBLE, &tipo_bloque);
    MPI_Type_create_resized(tipo_bloque, 0, (MPI_Aint)sizeof(double), &tipo_bloque_redim);
    MPI_Type_commit(&tipo_bloque_redim);

    if (rank == 0) {
        for (int r = 0; r < size; r++) {
            int c[11];
            MPI_Cart_coords(comm_2d, r, 2, c);
            int pos_offset = (c[6] * b) + (c * b) * n;

            if (r == 0) {
                for (int col = 0; col < b; col++) {
                    for (int fila = 0; fila < b; fila++) {
                        A_loc[fila + col * b] = A[fila + col * n];
                        B_loc[fila + col * b] = B[fila + col * n];
                    }
                }
            } else {
                MPI_Send(&A[pos_offset], 1, tipo_bloque_redim, r, 10, comm_2d);
                MPI_Send(&B[pos_offset], 1, tipo_bloque_redim, r, 20, comm_2d);
            }
        }
    } else {
        MPI_Recv(A_loc, b * b, MPI_DOUBLE, 0, 10, comm_2d, MPI_STATUS_IGNORE);
        MPI_Recv(B_loc, b * b, MPI_DOUBLE, 0, 20, comm_2d, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    for (int k = 0; k < q; k++) {
        if (mi_col == k) {
            memcpy(A_work, A_loc, (size_t)b * b * sizeof(double));
        }
        MPI_Bcast(A_work, b * b, MPI_DOUBLE, k, comm_row);

        if (mi_fila == k) {
            memcpy(B_work, B_loc, (size_t)b * b * sizeof(double));
        }
        MPI_Bcast(B_work, b * b, MPI_DOUBLE, k, comm_col);

        matmul_local_blocking_simd(A_work, B_work, C_loc, b);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();
    double t_par = t_end - t_start;

    if (rank == 0) {
        for (int r = 0; r < size; r++) {
            int c[11];
            MPI_Cart_coords(comm_2d, r, 2, c);
            int pos_offset = (c[6] * b) + (c * b) * n;

            if (r == 0) {
                for (int col = 0; col < b; col++) {
                    for (int fila = 0; fila < b; fila++) {
                        C[fila + col * n] = C_loc[fila + col * b];
                    }
                }
            } else {
                MPI_Recv(&C[pos_offset], 1, tipo_bloque_redim, r, 30, comm_2d, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(C_loc, b * b, MPI_DOUBLE, 0, 30, comm_2d);
    }

    if (rank == 0) {
        double gflops = (2.0 * n * n * n) / (t_par * 1e9);
        printf("\nResultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):\n");
        printf(" - Tiempo (Tpar): %.4f segundos\n", t_par);
        printf(" - Rendimiento: %.2f GFLOPS\n", gflops);

        if (check) {
            double error = 0.0;
            for (long i = 0; i < (long)n * n; i++) {
                double diff = C[i] - Cseq[i];
                error += diff * diff;
            }
            error = sqrt(error);
            printf(" - Speedup (S): %.4f\n", t_seq / t_par);
            printf(" - Error (norma euclídea): %.3e\n", error);
            free(Cseq);
        }
        free(A); free(B); free(C);
    }

    MPI_Type_free(&tipo_bloque);
    MPI_Type_free(&tipo_bloque_redim);
    free(A_loc); free(B_loc); free(C_loc);
    free(A_work); free(B_work);

    MPI_Finalize();
    return 0;
}