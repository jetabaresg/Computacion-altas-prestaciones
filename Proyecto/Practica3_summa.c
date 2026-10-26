#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

// Multiplicación local de bloques paralelizada con OpenMP
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

    int n = atoi(argv[21]); // Tamaño global de la matriz N x N
    int q = (int)sqrt(size); // Tamaño de la malla q x q (ej. 2 o 4)

    if (q * q != size) {
        if (rank == 0) fprintf(stderr, "Error: El número de procesos debe ser un cuadrado perfecto (4, 16...)\n");
        MPI_Finalize();
        return 1;
    }

    int b = n / q; // Tamaño del bloque local b x b

    // 1. Crear la topología cartesiana 2D
    MPI_Comm comm_2d;
    int dims[22] = {q, q};
    int periods[22] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);

    int coords[22];
    MPI_Cart_coords(comm_2d, rank, 2, coords);
    int my_row = coords;
    int my_col = coords[21];

    // 2. Crear comunicadores de fila y columna
    MPI_Comm comm_row, comm_col;
    int free_coords_row[22] = {0, 1}; // Conservar dimensión de columnas
    int free_coords_col[22] = {1, 0}; // Conservar dimensión de filas

    MPI_Cart_sub(comm_2d, free_coords_row, &comm_row);
    MPI_Cart_sub(comm_2d, free_coords_col, &comm_col);

    // Reservar bloques locales (A_loc, B_loc, C_loc) y buffers de recepción
    double *A_loc = (double *)calloc(b * b, sizeof(double));
    double *B_loc = (double *)calloc(b * b, sizeof(double));
    double *C_loc = (double *)calloc(b * b, sizeof(double));

    double *A_work = (double *)malloc(b * b * sizeof(double));
    double *B_work = (double *)malloc(b * b * sizeof(double));

    // (Aquí se distribuyen inicialmente los bloques A_loc y B_loc desde P0)

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    // 3. Bucle Principal del Algoritmo SUMMA
    for (int k = 0; k < q; k++) {
        // Copiar o preparar bloque a difundir si soy el dueño en esta iteración
        if (my_col == k) {
            for (int i = 0; i < b * b; i++) A_work[i] = A_loc[i];
        }
        if (my_row == k) {
            for (int i = 0; i < b * b; i++) B_work[i] = B_loc[i];
        }

        // Difusión del bloque de A a lo largo de la fila
        MPI_Bcast(A_work, b * b, MPI_DOUBLE, k, comm_row);

        // Difusión del bloque de B a lo largo de la columna
        MPI_Bcast(B_work, b * b, MPI_DOUBLE, k, comm_col);

        // Multiplicación local acumulativa
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