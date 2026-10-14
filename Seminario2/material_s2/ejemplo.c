#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const char *openmp_version_str(void) {
    int v = _OPENMP;
    int year = v / 100;
    int month = v % 100;
    const char *months[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                            "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    static char buf[64];
    if (month >= 1 && month <= 12)
        snprintf(buf, sizeof(buf), "Versi�n de OpenMP de %s de %d.", months[month], year);
    else
        snprintf(buf, sizeof(buf), "Versi�n de OpenMP: %d.", v);
    return buf;
}

int main(int argc, char *argv[]) {
    int nthreads = 0;
    if (argc == 2) {
        nthreads = atoi(argv[1]);
        if (nthreads > 0) omp_set_num_threads(nthreads);
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        int maxth = omp_get_max_threads();
        #pragma omp critical
        printf("Thread %d de %d (iniciados: %d)\n", tid, nth, maxth);
    }

    printf("%s\n", openmp_version_str());
    return 0;
}