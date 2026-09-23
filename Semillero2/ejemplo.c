#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        printf("Thread %d de %d\n", tid, nthreads);
    }

    printf("OpenMP: %d\n", _OPENMP);
    return 0;
}