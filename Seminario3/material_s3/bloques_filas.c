
/**************************************************
 * Producto matriz-vector distribuido 
 * con distribución cíclica por bloques de filas.
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define A(i,j) A[(i)*n+(j)]

int matrizvector( int m, int n, double *A, double *q, double *z );

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
  int n;
  sscanf(argv[1],"%d",&n);
  int blk;
  sscanf(argv[2],"%d",&blk);
  if( blk*size > n ) {
    if( rank==0 ) printf("blk debería ser más pequeño\n");
    MPI_Finalize();
    return 0;
  }

  double *A, *y, *y_orig;
  int blocks_per_process = (n+blk*size-1)/(blk*size);

  /* El proceso rank = 0 crea los datos */
  double *x = (double *) malloc( n*sizeof(double) );
  if( rank==0 ) {
    int m = blocks_per_process*blk*size; /* Número de filas total que hay que reservar (m>=n) */
    A = (double *) calloc( m*n, sizeof(double) );
    for( int i=0; i<n; i++ ) {
      A(i,i) = ( (double) rand() ) / RAND_MAX;
      for( int j=i+1; j<n; j++ ) {
        A(i,j) = A(j,i) = ( (double) rand() ) / RAND_MAX;
      }
    }
    for( int i=0; i<n; i++ ) {
      x[i] = ( (double) rand() ) / RAND_MAX;
    }
    y = (double *) malloc( m*sizeof(double) );
    /* El vector y_orig es para comprobar el resultado */
    /* y_orig = A * x */
    y_orig = (double *) malloc( m*sizeof(double) );
    matrizvector( m, n, A, x, y_orig );
  }
  
  /* Todos los procesos recibirán los datos en la matriz B */
  double *B = (double *) malloc( blocks_per_process*blk*n*sizeof(double) );

  /* Creación del tipo de dato MPI */
  . . .

  /* Envío de los datos: A y x */
  . . .

  /* Multiplicación a nivel local del nodo */
  . . .

  /* Hay que crear otro tipo de dato MPI para recoger al vector resultado */
  . . .

  /* Recogida del resultado en el vector y, tal que y = A * x */
  . . .


  /* Comprobación del resultado */
  if( rank == 0 ) {
    for( int i=0; i<n; i++ ) {
      y[i] -= y_orig[i];
    }
    printf("Error = %f\n",norm2(n,y));
  }

  free(x);
  free(B);
  if( rank==0 ) {
    free(y_orig);
    free(y);
    free(A);
  }
  MPI_Finalize();
  return 0;
}

#define B(i,j) B[(i)*n+(j)]
int matrizvector( int m, int n, double *B, double *q, double *z ) {
  for( int i=0; i<m; i++ ) {
    z[i] = 0.0;
    for( int j=0; j<n; j++ ) {
      z[i] += B(i,j)*q[j]; 
    }
  }
  return 0;
}

double norm2( int n, double *v ) {
  double x = 0.0;
  for( int i=0; i<n; i++ ) {
    x += v[i]*v[i];
  }
  return sqrt(x);
}


