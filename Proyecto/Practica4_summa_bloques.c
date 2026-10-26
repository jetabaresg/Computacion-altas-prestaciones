#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

// Parámetros de bloqueo para la memoria caché del nodo (ajustables)
#define MC 256
#define NC 256
#define KC 128

// Multiplicación local optimizada por bloques con empaquetamiento (BLIS / GotoBLAS)
void matmul_local_blocking(double *A, double *B, double *C, int b) {
    // b es el tamaño del bloque local que le corresponde al proceso en SUMMA
    
    for (int jc = 0; jc < b; jc += NC) {
        int nc_curr = (jc + NC > b) ? (b - jc) : NC;

        for (int pc = 0; pc < b; pc += KC) {
            int kc_curr = (pc + KC > b) ? (b - pc) : KC;

            // 1. Empaquetar bloque Bc (kc_curr x nc_curr) a memoria contigua
            double *Bc = (double *)malloc(kc_curr * nc_curr * sizeof(double));
            for (int j = 0; j < nc_curr; j++) {
                for (int k = 0; k < kc_curr; k++) {
                    Bc[k + j * kc_curr] = B[(pc + k) + (jc + j) * b];
                }
            }

            for (int ic = 0; ic < b; ic += MC) {
                int mc_curr = (ic + MC > b) ? (b - ic) : MC;

                // 2. Empaquetar bloque Ac (mc_curr x kc_curr) a memoria contigua
                double *Ac = (double *)malloc(mc_curr * kc_curr * sizeof(double));
                for (int k = 0; k < kc_curr; k++) {
                    for (int i = 0; i < mc_curr; i++) {
                        Ac[i + k * mc_curr] = A[(ic + i) + (pc + k) * b];
                    }
                }

                // 3. Multiplicación de los bloques Ac x Bc acumulando en C (paralelizada con OpenMP)
                #pragma omp parallel for default(shared)
                for (int j = 0; j < nc_curr; j++) {
                    for (int k = 0; k < kc_curr; k++) {
                        double b_val = Bc[k + j * kc_curr];
                        for (int i = 0; i < mc_curr; i++) {
                            C[(ic + i) + (jc + j) * b] += Ac[i + k * mc_curr] * b_val;
                        }
                    }
                }

                free(Ac);
            }
            free(Bc);
        }
    }
}