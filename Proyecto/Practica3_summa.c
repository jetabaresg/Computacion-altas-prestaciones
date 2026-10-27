#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

void matmul_local_block(double *A_blk, double *B_blk, double *C_blk, int b) {
    #pragma omp parallel for default(shared)
    for (int j = 0; j < b; j++) {
        for (int k = 0; k < b; k++) {
            double b_val = B_blk[k + j * b];
            for (int i = 0; i < b; i++) {
                C_blk[i + j * b] += A_blk[i + k * b] * b_val;
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
        if (rank == 0) printf("Uso: %s <N>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int q = (int)sqrt(size);

    if (q * q != size) {
        if (rank == 0) fprintf(stderr, "Error: El número de procesos debe ser un cuadrado perfecto (4, 16...)\n");
        MPI_Finalize();
        return 1;
    }

    int b = n / q; // Tamaño del bloque local b x b

    // 1. Topología cartesiana 2D (arrays de tamaño 2)
    MPI_Comm comm_2d;
    int dims[2] = {q, q};
    int periods[2] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);

    int coords[2];
    MPI_Cart_coords(comm_2d, rank, 2, coords);
    int my_row = coords[0];
    int my_col = coords[1];

    // 2. Comunicadores de fila y columna (arrays de tamaño 2)
    MPI_Comm comm_row, comm_col;
    int free_coords_row[2] = {0, 1}; // Conserva dimensión de columnas
    int free_coords_col[2] = {1, 0}; // Conserva dimensión de filas

    MPI_Cart_sub(comm_2d, free_coords_row, &comm_row);
    MPI_Cart_sub(comm_2d, free_coords_col, &comm_col);

    // 3. Reservar e inicializar bloques locales
    double *A_loc = (double *)malloc(b * b * sizeof(double));
    double *B_loc = (double *)malloc(b * b * sizeof(double));
    double *C_loc = (double *)calloc(b * b, sizeof(double));

    double *A_work = (double *)malloc(b * b * sizeof(double));
    double *B_work = (double *)malloc(b * b * sizeof(double));

    srand(12345 + rank);
    for (int i = 0; i < b * b; i++) {
        A_loc[i] = (double)(rand() % 10);
        B_loc[i] = (double)(rand() % 10);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    // 4. Bucle principal del algoritmo SUMMA
    for (int k = 0; k < q; k++) {
        if (my_col == k) {
            for (int i = 0; i < b * b; i++) A_work[i] = A_loc[i];
        }
        if (my_row == k) {
            for (int i = 0; i < b * b; i++) B_work[i] = B_loc[i];
        }

        MPI_Bcast(A_work, b * b, MPI_DOUBLE, k, comm_row);
        MPI_Bcast(B_work, b * b, MPI_DOUBLE, k, comm_col);

        matmul_local_block(A_work, B_work, C_loc, b);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();

    if (rank == 0) {
        double t_par = t_end - t_start;
        double gflops = (2.0 * n * n * n) / (t_par * 1e9);
        printf("SUMMA (Malla %dx%d) - N=%d - Tiempo: %.4f s - GFLOPS: %.2f\n", q, q, n, t_par, gflops);
    }

    free(A_loc); free(B_loc); free(C_loc);
    free(A_work); free(B_work);
    MPI_Finalize();
    return 0;
}