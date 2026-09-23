#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <limits.h>
#include <time.h>
#include <mpi.h> 
#include <errno.h>

#define MAX_NUMEROS 10000

#define MPI_SOLUCION_ENCONTRADA 0
#define MPI_BUSCAR_PRIMO 1
#define MPI_ESCLAVO_PREPARADO 2
#define MPI_FIN 3

#define ETIQUETA(tag) ((tag) == 0 ? "SOLUCION_ENCONTRADA" : (tag) == 1 ? "BUSCAR_PRIMO" : (tag) == 2 ? "ESCLAVO_PREPARADO" : "FIN")

typedef unsigned long long Entero_grande;
#define ENTERO_MAS_GRANDE  ULLONG_MAX

int es_primo( Entero_grande n ) {
  int p;
  Entero_grande i, s;

  p = (n % 2 != 0 || n == 2);

  if (p) {
    s = sqrt(n);

    for (i = 3; p && i <= s; i += 2)
      if (n % i == 0) p = 0;
  }

  return p;
}

Entero_grande generar_numero( ) {
  Entero_grande aux = rand();
  aux = ( aux << 32 ) + rand();
  if( !( aux % 2 ) ) aux++;
  return aux;
}

int main(int argc,char *argv[]) {
  int rank, size, primos_solicitados;
  MPI_Status status;

  MPI_Init(&argc,&argv); 
  MPI_Comm_size(MPI_COMM_WORLD,&size); 
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  if (size<2) {
    printf("El número minimo de procesos es de 2\n");
    MPI_Finalize(); 
    return 0;
  }
  
  if (argc >= 2) {
    char *fin;
    long solicitados;
    errno = 0;
    solicitados = strtol(argv[1], &fin, 10);
    if (errno != 0 || fin == argv[1] || *fin != '\0' || solicitados < 1 || solicitados > INT_MAX) {
      if (rank == 0) fprintf(stderr, "La cantidad de primos debe ser un entero positivo.\n");
      MPI_Finalize();
      return 1;
    }
    primos_solicitados = (int) solicitados;
  } else {
    primos_solicitados = 1;
  }

  if (rank == 0) {
      /* ------- Código del maestro ------- */
      double t1, t2;
      srand(time(NULL));
      t1 = MPI_Wtime();

      int k = 0;                  /* números primos encontrados */
      int esclavos = size - 1;    /* esclavos aún activos */
      Entero_grande numero;
      int tag, fuente;
      int iteracion = 0;

      while( esclavos > 0 ) {
        MPI_Recv(&numero, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        tag    = status.MPI_TAG;
        fuente = status.MPI_SOURCE;

        if( tag == MPI_SOLUCION_ENCONTRADA && k < primos_solicitados ) {
          printf("Primo %d encontrado: %llu\n", k+1, numero);
          k++;
        }

        if( k < primos_solicitados ) {
          numero = generar_numero();
          MPI_Send(&numero, 1, MPI_UNSIGNED_LONG_LONG, fuente, MPI_BUSCAR_PRIMO, MPI_COMM_WORLD);
        } else {
          MPI_Send(&numero, 1, MPI_UNSIGNED_LONG_LONG, fuente, MPI_FIN, MPI_COMM_WORLD);
          esclavos--;
        }
      }

      t2 = MPI_Wtime();
      printf("Tiempo = %f s.\n",t2-t1);
  } else { 
      /* ------- Código del esclavo ------- */
      int continuar = 1;
      Entero_grande n;
      MPI_Send(&n, 0, MPI_UNSIGNED_LONG_LONG, 0, MPI_ESCLAVO_PREPARADO, MPI_COMM_WORLD);

      while( continuar ) {
        MPI_Recv(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if( status.MPI_TAG == MPI_FIN ) {
          continuar = 0;
        } else if( es_primo(n) ) {
          MPI_Send(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_SOLUCION_ENCONTRADA, MPI_COMM_WORLD);
        } else {
          MPI_Send(&n, 0, MPI_UNSIGNED_LONG_LONG, 0, MPI_ESCLAVO_PREPARADO, MPI_COMM_WORLD);
        }
      }
  }
  
  MPI_Finalize(); 
  return 0;
}