
#include <stdio.h>
#include <mpi.h>

int main( int argc, char *argv[] ) {

  const int ndims = 2;
  int ierr;
  int p, rank, cart_rank;
  MPI_Comm comm2D;
  int dims[ndims], coord[ndims];
  int wrap_around[ndims];
  int reorder, nrows, ncols;

  MPI_Init(&argc,&argv);
  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  if( argc<3 ) {
    if( rank==0 ) printf("Usage: mpirun -np %d %s nrows ncols\n",p,argv[0]);
    MPI_Finalize();
    return 0;
  }
  sscanf(argv[1],"%d",&nrows);
  sscanf(argv[2],"%d",&ncols);
  if( nrows*ncols > p ) {
    if( rank==0 ) printf("%d debe ser >= %d\n",p,nrows*ncols);
    MPI_Finalize();
    return 0;
  }

  /*
     Código
   */

  MPI_Comm_free(&comm2D);
  MPI_Finalize();
  return 0;
}

