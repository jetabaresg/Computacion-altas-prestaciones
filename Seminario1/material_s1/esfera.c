#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 
#include <mpi.h> 
#include <errno.h>

int main(int argc,char *argv[]) {
  int rank, size;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

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

  srand(rank+1);
  unsigned long int aciertos_parcial = 0;
  for( unsigned long int i = 0; i<puntos_por_proceso; i++ ) {
    double x = (((double) rand() ) / RAND_MAX ) * radio;
    double y = (((double) rand() ) / RAND_MAX ) * radio;
    double z = (((double) rand() ) / RAND_MAX ) * radio;
    if( ( x*x + y*y + z*z ) <= (radio*radio) ) aciertos_parcial+=1.0;
  }

  unsigned long int ac_totales = 0;
  if (rank != 0) {
    MPI_Send(&aciertos_parcial, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
  } else {
    unsigned long int tmp;
    ac_totales = aciertos_parcial;
    for( int p = 1; p < size; p++ ) {
      MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      ac_totales += tmp;
    }
  }

  if (rank==0) {
    unsigned long int max_total = puntos_por_proceso * size;
    double volumen = radio * radio * radio * (double)ac_totales / max_total * 8;
    printf("Aciertos totales = %lu de %lu\n", ac_totales, max_total);
    printf("Volumen calculado = %f, volumen esfera = %f. \n", volumen, 4.0*M_PI*radio*radio*radio/3.0);
  }

  MPI_Finalize();
  return 0;
}