
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main( int argc, char *argv[] ) {

  struct tipo_punto {
    int i;
    double x, y;
    double masa;
    char nombre[10];
  };

  struct tipo_punto punto_a_enviar, punto_a_recibir;

  int rank, size;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  /****************************************
   Construcción del tipo de dato MPI
   ***************************************/

  if( rank==0 ) {
    punto_a_enviar.i = 100;
    punto_a_enviar.x = 1.3;
    punto_a_enviar.y = 1.8;
    punto_a_enviar.masa = 2.1E-6;
    strcpy(punto_a_enviar.nombre,"muon01");
  }

  /****************************************
   Envio del dato al anillo y recepción
   ***************************************/

  /* Los valores deben ser los mismos */
  if( rank==0 ) {
    printf("i = %d\n",punto_a_recibir.i);
    printf("x = %f\n",punto_a_recibir.x);
    printf("y = %f\n",punto_a_recibir.y);
    printf("masa = %e\n",punto_a_recibir.masa);
    printf("nombre = %s\n",punto_a_recibir.nombre);
  }

  /****************************************
   Destrucción del tipo de dato MPI
   ***************************************/

  MPI_Finalize();
  return 0;
}

