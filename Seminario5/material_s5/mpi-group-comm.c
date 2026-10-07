
/*****************************************
 * mpi-create-comm-group.c --
 * builds a communicator from the first q processes
 * in a communicator containing arbitrary number of processes.
 * Written by Mary Thomas
 * - Updated Mar, 2015
 * Based loosely on code from Pacheco’97, See Chap 7,PPMPI
 * Example: mpi-create-comm-group.c (1/3)
 */

#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>
#include <mpi.h> 

int main( int argc, char *argv[] ) {
  int p, q; /* sqrt(p) */
  int rank, comm_rank;
  MPI_Group group_world;
  MPI_Group comm1_group, comm2_group;
  MPI_Comm comm1, comm2;
  int *comm1_procs, *comm2_procs;
  int err;
  int test;
  int sum;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if( p < 2 ) {
    if( rank ==0 ) printf("ERROR: The number of processes %d must be >= 2 \n",p);
    MPI_Finalize();
    return(0); 
  }

  /**********************************************
   *
   Código
   *
   *********************************************/

  MPI_Finalize();
  return 0;

}

