#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#define MC 256
#define NC 256
#define KC 128

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

void matmul_local_bloques(double *A, double *B, double *C, int b) {
    for (int jc = 0; jc < b; jc += NC) {
        int nc_actual = (jc + NC > b) ? (b - jc) : NC;

        for (int pc = 0; pc < b; pc += KC) {
            int kc_actual = (pc + KC > b) ? (b - pc) : KC;

            double *Bc = (double *)malloc((size_t)kc_actual * nc_actual * sizeof(double));
            for (int j = 0; j < nc_actual; j++) {
                for (int k = 0; k < kc_actual; k++) {
                    Bc[k + j * kc_actual] = B[(pc + k) + (jc + j) * b];
                }
            }

            for (int ic = 0; ic < b; ic += MC) {
                int mc_actual = (ic + MC > b) ? (b - ic) : MC;

                double *Ac = (double *)malloc((size_t)mc_actual * kc_actual * sizeof(double));
                for (int k = 0; k < kc_actual; k++) {
                    for (int i = 0; i < mc_actual; i++) {
                        Ac[i + k * mc_actual] = A[(ic + i) + (pc + k) * b];
                    }
                }

                #pragma omp parallel for default(shared) private(k, i)
                for (int j = 0; j < nc_actual; j++) {
                    for (int k = 0; k < kc_actual; k++) {
                        double valor_b = Bc[k + j * kc_actual];
                        for (int i = 0; i < mc_actual; i++) {
                            C[(ic + i) + (jc + j) * b] += Ac[i + k * mc_actual] * valor_b;
                        }
                    }
                }

                free(Ac);
            }
            free(Bc);
        }
    }
}

int main(int argc, char *argv[]) {
    int rango, tamano;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tamano);

    if (argc < 2) {
        if (rango == 0) printf("Uso: %s <tamano_matriz_N> [check]\n", argv);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int verificar = (argc > 2) && (strcmp(argv[2], "check") == 0);

    int q = (int)sqrt(tamano);
    if (q * q != tamano) {
        if (rango == 0) fprintf(stderr, "Error: El número de procesos (%d) debe ser un cuadrado perfecto.\n", tamano);
        MPI_Finalize();
        return 1;
    }

    if (n % q != 0) {
        if (rango == 0) fprintf(stderr, "Error: N (%d) debe ser múltiplo de la dimensión de la malla (%d).\n", n, q);
        MPI_Finalize();
        return 1;
    }

    int b = n / q;

    int dims[2] = {q, q};
    int periodos[2] = {0, 0};
    MPI_Comm comm_2d;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodos, 1, &comm_2d);

    int coords[2];
    MPI_Cart_coords(comm_2d, rango, 2, coords);
    int mi_fila = coords;
    int mi_col = coords[1];

    MPI_Comm comm_fila, comm_col;
    int dims_fila[2] = {0, 1};
    int dims_col[2] = {1, 0};
    MPI_Cart_sub(comm_2d, dims_fila, &comm_fila);
    MPI_Cart_sub(comm_2d, dims_col, &comm_col);

    double *A_local = (double *)calloc((size_t)b * b, sizeof(double));
    double *B_local = (double *)calloc((size_t)b * b, sizeof(double));
    double *C_local = (double *)calloc((size_t)b * b, sizeof(double));
    double *A_buff  = (double *)malloc((size_t)b * b * sizeof(double));
    double *B_buff  = (double *)malloc((size_t)b * b * sizeof(double));

    double *A = NULL, *B = NULL, *C = NULL, *Cseq = NULL;
    double t_seq = 0.0;

    if (rango == 0) {
        printf("=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===\n");
        printf("Matriz N = %d | Malla 2D = %dx%d (%d procesos) | Bloque local b = %d\n", n, q, q, tamano, b);

        A = (double *)malloc((size_t)n * n * sizeof(double));
        B = (double *)malloc((size_t)n * n * sizeof(double));
        C = (double *)malloc((size_t)n * n * sizeof(double));

        inicializar_matriz_col(A, n, 12345u);
        inicializar_matriz_col(B, n, 67890u);

        if (verificar) {
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

    if (rango == 0) {
        for (int r = 0; r < tamano; r++) {
            int c[2];
            MPI_Cart_coords(comm_2d, r, 2, c);
            int pos_offset = (c * b) + (c[1] * b) * n;

            if (r == 0) {
                for (int col = 0; col < b; col++) {
                    for (int fila = 0; fila < b; fila++) {
                        A_local[fila + col * b] = A[fila + col * n];
                        B_local[fila + col * b] = B[fila + col * n];
                    }
                }
            } else {
                MPI_Send(&A[pos_offset], 1, tipo_bloque_redim, r, 10, comm_2d);
                MPI_Send(&B[pos_offset], 1, tipo_bloque_redim, r, 20, comm_2d);
            }
        }
    } else {
        MPI_Recv(A_local, b * b, MPI_DOUBLE, 0, 10, comm_2d, MPI_STATUS_IGNORE);
        MPI_Recv(B_local, b * b, MPI_DOUBLE, 0, 20, comm_2d, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double tpar_inicio = MPI_Wtime();

    for (int k_paso = 0; k_paso < q; k_paso++) {
        if (mi_col == k_paso) {
            memcpy(A_buff, A_local, (size_t)b * b * sizeof(double));
        }
        MPI_Bcast(A_buff, b * b, MPI_DOUBLE, k_paso, comm_fila);

        if (mi_fila == k_paso) {
            memcpy(B_buff, B_local, (size_t)b * b * sizeof(double));
        }
        MPI_Bcast(B_buff, b * b, MPI_DOUBLE, k_paso, comm_col);

        matmul_local_bloques(A_buff, B_buff, C_local, b);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double tpar_fin = MPI_Wtime();
    double t_par = tpar_fin - tpar_inicio;

    if (rango == 0) {
        for (int r = 0; r < tamano; r++) {
            int c[2];
            MPI_Cart_coords(comm_2d, r, 2, c);
            int pos_offset = (c * b) + (c[1] * b) * n;

            if (r == 0) {
                for (int col = 0; col < b; col++) {
                    for (int fila = 0; fila < b; fila++) {
                        C[fila + col * n] = C_local[fila + col * b];
                    }
                }
            } else {
                MPI_Recv(&C[pos_offset], 1, tipo_bloque_redim, r, 30, comm_2d, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(C_local, b * b, MPI_DOUBLE, 0, 30, comm_2d);
    }

    if (rango == 0) {
        double gflops_par = (2.0 * n * n * n) / (t_par * 1e9);
        printf("\nResultado Paralelo (SUMMA + BLIS):\n");
        printf(" - Tiempo (Tpar): %.4f segundos\n", t_par);
        printf(" - Rendimiento: %.2f GFLOPS\n", gflops_par);

        if (verificar) {
            double error = 0.0;
            for (long k = 0; k < (long)n * n; k++) {
                double diff = C[k] - Cseq[k];
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
    free(A_local); free(B_local); free(C_local);
    free(A_buff); free(B_buff);

    MPI_Finalize();
    return 0;
}