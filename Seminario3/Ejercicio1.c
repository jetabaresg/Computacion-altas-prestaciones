#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <errno.h>
#include <omp.h>
#include <stdint.h>

static inline uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *state = x;
    return x;
}

static inline double xorshift64_double(uint64_t *state) {
    return (double)(xorshift64(state) >> 11) * (1.0 / 9007199254740992.0);
}

int main(int argc,char *argv[]) {
    int rank, size;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    double t_total_start = MPI_Wtime();

    if (argc<2) {
        if (rank==0) printf("Uso: %s radio [max_puntos]\n",argv[0]);
        MPI_Finalize();
        return 0;
    }

    char *fin;
    errno = 0;
    double radio = strtod(argv[1], &fin);
    if (errno != 0 || fin == argv[1] || *fin != '\0' || radio <= 0.0) {
        if (rank == 0) fprintf(stderr, "El radio debe ser un número mayor que cero.\n");
        MPI_Finalize();
        return 1;
    }

    double max_puntos = 10E7;
    if (argc > 2) {
        errno = 0;
        max_puntos = strtod(argv[2], &fin);
        if (errno != 0 || fin == argv[2] || *fin != '\0' || max_puntos < 1.0) {
            if (rank == 0) fprintf(stderr, "max_puntos debe ser un número mayor que cero.\n");
            MPI_Finalize();
            return 1;
        }
    }

    unsigned long int puntos_por_proceso = (unsigned long int)(max_puntos / size);
    if (puntos_por_proceso < 1) puntos_por_proceso = 1;

    if (rank==0) {
        printf("Calculo del volumen de una esfera de radio %.2f (max_puntos = %.0e) con %d procesos\n",radio,max_puntos,size);
    }

    double t_comp_start = MPI_Wtime();

    unsigned long int aciertos_parcial = 0;
    unsigned long int ac_totales = 0;
    for( unsigned long int i = 0; i<puntos_por_proceso; i++ ) {
        double x, y, z;
        uint64_t seed = ((uint64_t)(rank + 1) << 32) ^ (uint64_t)omp_get_thread_num() ^ 0x9e3779b97f4a7c15ULL;
        x = xorshift64_double(&seed) * radio;
        y = xorshift64_double(&seed) * radio;
        z = xorshift64_double(&seed) * radio;
        if( ( x*x + y*y + z*z ) <= (radio*radio) ){
            aciertos_parcial++;
        }
    }

    double t_comp_end = MPI_Wtime();

    MPI_Reduce(&aciertos_parcial, &ac_totales, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        unsigned long int max_total = puntos_por_proceso * size;
        double volumen = radio * radio * radio * (double)ac_totales / max_total * 8.0;
        double t_total_end = MPI_Wtime();

        printf("Aciertos totales = %lu de %lu\n", ac_totales, max_total);
        printf("Volumen calculado = %f, volumen esfera = %f\n", volumen, 4.0 * M_PI * radio * radio * radio / 3.0);
        printf("Tiempos: total=%.4f s  comp=%.4f s\n",
               t_total_end - t_total_start, t_comp_end - t_comp_start);
    }
    MPI_Finalize();
    return 0;
}