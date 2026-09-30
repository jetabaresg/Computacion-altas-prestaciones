#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define A(i,j) A[(j)*m+(i)]
#define Asaved(i,j) Asaved[(j)*m+(i)]

double norm2( int n, double *v );

int main( int argc, char *argv[] ) {

  MPI_Init(&argc,&argv);
  int size, rank;
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  if( argc<3 ) {
    if( rank==0 ) printf("Usage: mpirun -np %d %s n block_size\n",size,argv[0]);
    MPI_Finalize();
    return 0;
  }
  int m, n;
  sscanf(argv[1],"%d",&m);
  int blk;
  sscanf(argv[2],"%d",&blk);
  if( blk*size > m ) {
    if( rank==0 ) printf("blk debería ser más pequeño\n");
    MPI_Finalize();
    return 0;
  }

  int blocks_per_process = (m+blk*size-1)/(blk*size);
  n = blocks_per_process * blk * size;

  double *A = NULL, *Asaved = NULL;
  if( rank==0 ) {
    A = (double *) calloc( m*n, sizeof(double) );
    Asaved = (double *) calloc( m*n, sizeof(double) );
    for( int i=0; i<m; i++ ) {
      A(i,i) = Asaved( i, i ) = ( (double) rand() ) / RAND_MAX;
      for( int j=i+1; j<m; j++ ) {
        A(i,j) = A(j,i) = Asaved(i,j) = Asaved(j,i) = ( (double) rand() ) / RAND_MAX;
      }
    }
  }

  int local_cols = blocks_per_process * blk;
  double *B = (double *) malloc( m * local_cols * sizeof(double) );

  MPI_Datatype bloque, tipo_envio;
  MPI_Type_vector( blocks_per_process, blk*m, blk*size*m, MPI_DOUBLE, &bloque );
  MPI_Type_create_resized( bloque, 0,
      (MPI_Aint)blocks_per_process*blk*m*sizeof(double), &tipo_envio );
  MPI_Type_commit( &tipo_envio );

  if (rank == 0) {
    MPI_Scatter( A, 1, tipo_envio, B, m*local_cols, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  } else {
    MPI_Scatter( NULL, 1, tipo_envio, B, m*local_cols, MPI_DOUBLE, 0, MPI_COMM_WORLD );
  }

  if (rank == 0) {
    MPI_Gather( B, m*local_cols, MPI_DOUBLE, A, 1, tipo_envio, 0, MPI_COMM_WORLD );
  } else {
    MPI_Gather( B, m*local_cols, MPI_DOUBLE, NULL, 1, tipo_envio, 0, MPI_COMM_WORLD );
  }

  if( rank == 0 ) {
    for( int i=0; i<m; i++ ) {
      for( int j=0; j<n; j++ ) {
        Asaved( i, j ) -= A( i, j );
      }
    }
    printf("Error = %f\n",norm2(m*n,Asaved));
  }

  MPI_Type_free( &bloque );
  MPI_Type_free( &tipo_envio );
  free(B);
  if( rank==0 ) {
    free(Asaved);
    free(A);
  }
  MPI_Finalize();
  return 0;
}

double norm2( int n, double *v ) {
  double x = 0.0;
  for( int i=0; i<n; i++ ) {
    x += v[i]*v[i];
  }
  return sqrt(x);
}