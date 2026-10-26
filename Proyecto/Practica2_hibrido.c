#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

void init_matrix_col_major(double *M, int n, unsigned int seed) {
    srand(seed);
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            M[i + j * n] = (double)(rand() % 10);
        }
    }
}

void matmul_secuencial_col(double *A, double *B, double *C, int n, int m) {
    #pragma omp parallel for default(shared)
    for (int j = 0; j < m; j++) {
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

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Uso: %s <tamano_matriz_n> [check]\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int check = (argc > 2) && (strcmp(argv[2], "check") == 0);

    if (n <= 0 || n % size != 0) {
        if (rank == 0) {
            fprintf(stderr,
                    "Error: n (%d) debe ser positivo y multiplo del numero de procesos (%d)\n",
                    n, size);
        }
        MPI_Finalize();
        return 1;
    }

    int m = n / size;

    double *A  = (double *)malloc((size_t)n * n * sizeof(double));
    double *Bi = (double *)malloc((size_t)n * m * sizeof(double));
    double *Ci = (double *)malloc((size_t)n * m * sizeof(double));

    double *B = NULL, *C = NULL, *Cseq = NULL;
    double t_seq = 0.0;

    if (rank == 0) {
        printf("=== PRACTICA 1 ===\n");
        printf("Proceso P0 ejecutando con N = %d y %d procesos MPI iniciados.\n",n, size);

        B = (double *)malloc((size_t)n * n * sizeof(double));
        C = (double *)malloc((size_t)n * n * sizeof(double));

        init_matrix_col_major(A, n, 12345u);
        init_matrix_col_major(B, n, 67890u);

        if (check) {
            Cseq = (double *)malloc((size_t)n * n * sizeof(double));
            double t_start = MPI_Wtime();
            matmul_secuencial_col(A, B, Cseq, n, n);
            double t_end = MPI_Wtime();

            t_seq = t_end - t_start;
            double gflops_seq = (2.0 * n * n * n) / (t_seq * 1e9);

            printf("Resultado Secuencial:\n");
            printf(" - Tiempo (Tseq): %.4f segundos\n", t_seq);
            printf(" - Rendimiento: %.2f GFLOPS\n", gflops_seq);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double tpar_start = MPI_Wtime();

    MPI_Bcast(A, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Scatter(B, n * m, MPI_DOUBLE,
                Bi, n * m, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    matmul_secuencial_col(A, Bi, Ci, n, m);

    MPI_Gather(Ci, n * m, MPI_DOUBLE,C, n * m, MPI_DOUBLE,0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double tpar_end = MPI_Wtime();
    double t_par = tpar_end - tpar_start;

    if (rank == 0) {
        double gflops_par = (2.0 * n * n * n) / (t_par * 1e9);

        printf("\nResultado Paralelo (MPI):\n");
        printf(" - Tiempo (Tpar): %.4f segundos\n", t_par);
        printf(" - Rendimiento: %.2f GFLOPS\n", gflops_par);

        if (check) {
            double S = t_seq / t_par;
            double E = S / size;

            double err = 0.0;
            for (long k = 0; k < (long)n * n; k++) {
                double d = C[k] - Cseq[k];
                err += d * d;
            }
            err = sqrt(err);

            printf(" - Speedup (S): %.4f\n", S);
            printf(" - Eficiencia (E): %.4f\n", E);
            printf(" - Error (norma euclidea C - Cseq): %.3e\n", err);
        }
    }

    free(A);
    free(Bi);
    free(Ci);
    if (rank == 0) {
        free(B);
        free(C);
        if (check) free(Cseq);
    }

    MPI_Finalize();
    return 0;
}