#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define A(i,j) A[(i)+(j)*n]

static int potencia_p2p( int n, double *A, double *lambda, double *q );
static int potencia_coll( int n, double *A, double *lambda, double *q );

int main( int argc, char *argv[] ) {
    double t_start = MPI_Wtime();
    MPI_Init(&argc,&argv);
    int nprocs, rank;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    if( argc<2 ) {
        if( rank==0 ) printf("Usage: mpirun -np %d %s n\n",nprocs,argv[0]);
        MPI_Finalize();
        return 0;
    }

    int n;
    sscanf(argv[1],"%d",&n);

    double *A = NULL;
    if( rank==0 ) {
        int cols_per_proc = n/nprocs + (n%nprocs>0?1:0);
        int total_cols = cols_per_proc*nprocs;
        A = (double *) malloc( n*total_cols*sizeof(double) );
        for( int i=0; i<n; i++ ) {
            A(i,i) = ( (double) rand() ) / RAND_MAX;
            for( int j=i+1; j<n; j++ ) {
                A(i,j) = A(j,i) = ( (double) rand() ) / RAND_MAX;
            }
        }
    }
    double *q = (double *) malloc( n*sizeof(double) );

    double lambda1, lambda2;
    int k1 = potencia_p2p( n, A, &lambda1, q );
    MPI_Barrier(MPI_COMM_WORLD);
    int k2 = potencia_coll( n, A, &lambda2, q );

    if( rank==0 ) {
        printf("Punto a punto: lambda = %f, iter = %d\n", lambda1, k1);
        printf("Colectivas:    lambda = %f, iter = %d\n", lambda2, k2);
        double t_end = MPI_Wtime();
        printf("Tiempo total = %.4f s\n", t_end - t_start);
        free(q);
        free(A);
    }

    MPI_Finalize();
    return 0;
}

int matrizvector_p2p( int n, int cols_per_proc, double *A, double *q, double *z ) {
    int nprocs, rank;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    for( int i=0; i<n; i++ ) {
        z[i] = 0.0;
        for( int j=0; j<cols_per_proc; j++ ) {
            z[i] += A(i,j)*q[j];
        }
    }
    if( rank == 0 ) {
        double *r = (double *) malloc( n*sizeof(double) );
        for( int p=1; p<nprocs; p++ ) {
            MPI_Recv( r, n, MPI_DOUBLE, p, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            for( int i=0; i<n; i++ ) z[i] += r[i];
        }
        free(r);
        for( int p=1; p<nprocs; p++ ) {
            MPI_Send( z, n, MPI_DOUBLE, p, 123, MPI_COMM_WORLD );
        }
    } else {
        MPI_Send( z, n, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD );
        MPI_Recv( z, n, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }
    return 0;
}

int matrizvector_coll( int n, int cols_per_proc, double *A, double *q, double *z ) {
    int nprocs, rank;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    for( int i=0; i<n; i++ ) {
        z[i] = 0.0;
        for( int j=0; j<cols_per_proc; j++ ) {
            z[i] += A(i,j)*q[j];
        }
    }
    double *zglobal = (double *) malloc( n*sizeof(double) );
    MPI_Reduce( z, zglobal, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
    MPI_Bcast( zglobal, n, MPI_DOUBLE, 0, MPI_COMM_WORLD );
    for( int i=0; i<n; i++ ) z[i] = zglobal[i];
    free(zglobal);
    return 0;
}

double norm2( int n, double *v ) {
    double x = 0.0;
    for( int i=0; i<n; i++ ) x += v[i]*v[i];
    return sqrt(x);
}

int scal( int n, double alfa, double *v, double *w ) {
    for( int i=0; i<n; i++ ) w[i] = alfa*v[i];
    return 0;
}

double dot( int n, double *v, double *w ) {
    double d = 0.0;
    for( int i=0; i<n; i++ ) d += v[i]*w[i];
    return d;
}

static int potencia_p2p( int n, double *A, double *lambda, double *q ) {
    int nprocs, rank;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    int cols_per_proc = n/nprocs + (n%nprocs>0?1:0);

    double *Aloc = (double *) malloc( n*cols_per_proc*sizeof(double) );
    if( rank==0 ) {
        for( int p=1; p<nprocs; p++ ) {
            MPI_Send( &A[p*n*cols_per_proc], n*cols_per_proc, MPI_DOUBLE, p, 123, MPI_COMM_WORLD );
        }
        MPI_Sendrecv( A, n*cols_per_proc, MPI_DOUBLE, 0, 123, Aloc, n*cols_per_proc, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    } else {
        MPI_Recv( Aloc, n*cols_per_proc, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }

    double *z = (double *) malloc( n*sizeof(double) );
    double alfa = 0.0;
    int k = 1;

    if( rank == 0 ) {
        for( int p=1; p<nprocs; p++ ) {
            MPI_Send( &A(0,0), n, MPI_DOUBLE, p, 123, MPI_COMM_WORLD );
        }
        MPI_Sendrecv( &A(0,0), n, MPI_DOUBLE, 0, 123, z, n, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    } else {
        MPI_Recv( z, n, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }
    if( rank == 0 ) {
        for( int p=1; p<nprocs; p++ ) {
            MPI_Send( &A(0,0), 1, MPI_DOUBLE, p, 123, MPI_COMM_WORLD );
        }
        MPI_Sendrecv( &A(0,0), 1, MPI_DOUBLE, 0, 123, lambda, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    } else {
        MPI_Recv( lambda, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }

    while( fabs(*lambda-alfa)>2.220446049250313e-16 ) {
        k++;
        alfa = *lambda;
        scal( n, 1.0/norm2(n,z), z, q );
        matrizvector_p2p( n, cols_per_proc, Aloc, &q[rank*cols_per_proc], z );
        *lambda = dot( n, q, z );
    }

    free(Aloc);
    free(z);
    return k;
}

static int potencia_coll( int n, double *A, double *lambda, double *q ) {
    int nprocs, rank;
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    int cols_per_proc = n/nprocs + (n%nprocs>0?1:0);

    double *Aloc = (double *) malloc( n*cols_per_proc*sizeof(double) );
    MPI_Scatter( A, n*cols_per_proc, MPI_DOUBLE, Aloc, n*cols_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD );

    double *z = (double *) malloc( n*sizeof(double) );
    if (rank == 0) {
        for (int i = 0; i < n; i++) z[i] = A(i,0);
    }
    MPI_Bcast( z, n, MPI_DOUBLE, 0, MPI_COMM_WORLD );

    double lambda_local = 0.0;
    if (rank == 0) lambda_local = A(0,0);
    MPI_Bcast( &lambda_local, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
    *lambda = lambda_local;

    double alfa = 0.0;
    int k = 1;

    while( fabs(*lambda-alfa)>2.220446049250313e-16 ) {
        k++;
        alfa = *lambda;
        scal( n, 1.0/norm2(n,z), z, q );
        matrizvector_coll( n, cols_per_proc, Aloc, &q[rank*cols_per_proc], z );
        *lambda = dot( n, q, z );
    }

    free(Aloc);
    free(z);
    return k;
}