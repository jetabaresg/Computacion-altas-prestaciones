#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "ctimer.h"

#define A(i,j) A[(j)*n+(i)]

int cholesky_bloques_par( int n, int b, double *A, int nthreads );

int main( int argc, char *argv[] ) {
    if( argc < 3 ) {
        fprintf(stderr,"usage: %s n block_size [nthreads]\n",argv[0]);
        exit(-1);
    }
    int n = atoi(argv[1]);
    int b = atoi(argv[2]);
    int nthreads = (argc >= 4) ? atoi(argv[3]) : omp_get_max_threads();
    if (nthreads > 0) omp_set_num_threads(nthreads);

    double *A = (double*) malloc(n*n*sizeof(double));
    double *L = (double*) malloc(n*n*sizeof(double));

    for( int j=0; j<n; j++ ) {
        for( int i=0; i<j; i++ ) L(i,j) = 0.0;
        for( int i=j; i<n; i++ ) L(i,j) = ((double)rand()) / RAND_MAX;
        L(j,j) += n;
    }

    double zero = 0.0, one = 1.0;
    dsyrk_( "L", "N", &n, &n, &one, L, &n, &zero, A, &n );

    double t1, t2, ucpu, scpu;
    ctimer( &t1, &ucpu, &scpu );
    int info = cholesky_bloques_par( n, b, A, nthreads );
    ctimer( &t2, &ucpu, &scpu );

    if( info != 0 ) {
        fprintf(stderr,"Error = %d en descomposición Cholesky\n",info);
        exit(-1);
    }

    double error = 0.0;
    for( int j=0; j<n; j++ )
        for( int i=j; i<n; i++ ) {
            double d = A(i,j) - L(i,j);
            error += d*d;
        }
    error = sqrt(error);

    printf("%d %d %d %.4f %.4e\n", n, b, nthreads, t2-t1, error);

    free(A); free(L);
    return 0;
}

int cholesky_bloques_par( int n, int b, double *A, int nthreads ) {
    int i, j, k, m;
    int info = 0;
    const double one = 1.0;
    const double minusone = -1.0;

    for ( k = 0; k < n; k += b ) {
        m = (n - k < b) ? n - k : b;
        dpotrf_( "L", &m, &A(k,k), &n, &info );
        if( info != 0 ) return info;

        #pragma omp parallel for schedule(static)
        for ( i = k + b; i < n; i += b ) {
            m = (n - i < b) ? n - i : b;
            dtrsm_( "R", "L", "T", "N", &m, &b, &one, &A(k,k), &n, &A(i,k), &n );
        }

        #pragma omp parallel for schedule(static) collapse(2)
        for ( i = k + b; i < n; i += b ) {
            int mi = (n - i < b) ? n - i : b;
            for ( j = k + b; j < i; j += b ) {
                int mj = (n - j < b) ? n - j : b;
                dgemm_( "N", "T", &mi, &mj, &b, &minusone, &A(i,k), &n, &A(j,k), &n, &one, &A(i,j), &n );
            }
        }

        #pragma omp parallel for schedule(static)
        for ( i = k + b; i < n; i += b ) {
            int mi = (n - i < b) ? n - i : b;
            dsyrk_( "L", "N", &mi, &b, &minusone, &A(i,k), &n, &one, &A(i,i), &n );
        }
    }
    return 0;
}