#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ctimer.h"

#define Cseq(i,j) Cseq[(i)+m*(j)]
#define C(i,j) C[(i)+m*(j)]
#define A(i,j) A[(i)+m*(j)]
#define B(i,j) B[(i)+k*(j)]

void print( const char *matriz, const int m, const int n, double *M ) {
  printf("%s = [\n",matriz);
  for( int i = 0; i<m; i++ ) {
    for( int j = 0; j<n; j++ ) {
      printf("%9.4f", M[i+m*j]);
    }
    printf("\n");
  }
  printf("]\n");
}
// Empaqueta el bloque B(pc:pc+kc, jc:jc+nc) en memoria contigua 
//Bc 
void pack_B(int kc, int nc, double *B, int ldb, double *Bc) {
  for (int j = 0; j < nc; j++) {
    for (int p = 0; p < kc; p++) {
      Bc[p + j * kc] = B[p + j * ldb]; 
    } 
  } 
}
// Empaqueta el bloque A(ic:ic+mc, pc:pc+kc) en memoria contigua
// Ac: mc x kc column-major
void pack_A(int mc, int kc, double *A, int lda, double *Ac) {
  for (int p = 0; p < kc; p++) {
    for (int i = 0; i < mc; i++) {
      Ac[i + p * mc] = A[i + p * lda]; 
    } 
  } 
}

int main(int argc, char *argv[]){

  if( argc<2 ) {
    printf("Usage: <m> <n> <p> seq? <mc> <nc> <kc>\n");
    return 1;
  }
  int m, n, k;
  sscanf(argv[1],"%d",&m);
  if( argc>2 ) {
    sscanf(argv[2],"%d",&n);
  } else { n = k = m; }
  if( argc>3 ) {
    sscanf(argv[3],"%d",&k);
  } else { k = m; }
  int seq = 0;
  if( argc>4 ) {
    sscanf(argv[4],"%d",&seq);
  } 
  // para leer en la terminal 
  int mc = 256, nc = 256, kc = 256;
  if (argc > 5) sscanf(argv[5], "%d", &mc);
  if (argc > 6) sscanf(argv[6], "%d", &nc); 
  if (argc > 7) sscanf(argv[7], "%d", &kc);

  double *A = (double *) malloc (m*k*sizeof(double));
  double *B = (double *) malloc (k*n*sizeof(double));
  double *Cseq = (double *) malloc (m*n*sizeof(double));
  double *C = (double *) malloc (m*n*sizeof(double));

  //Inicializar matrices 
  for( int i = 0; i<m*k; i++ ) {
    A[i] = (double) rand()/RAND_MAX * 2.0 - 1.0;
  }
  for( int i = 0; i<k*n; i++ ) {
    B[i] = (double) rand()/RAND_MAX * 2.0 - 1.0;
  }
  for( int i = 0; i<k*n; i++ ) {
    C[i] = Cseq[i] = 0;
  }
  // print( "A", m, k, A );
  // print( "B", k, n, B );

  if( seq ) {
    double elapsed, ucpu, scpu;
    ctimer( &elapsed, &ucpu, &scpu );
    for( int j = 0; j < n; j++ ) {
      for( int p = 0; p < k; p++ ) {
          for( int i = 0; i < m; i++ ) {
             Cseq(i,j) = Cseq(i,j) + A(i,p) * B(p,j); 
          }
      }
    }
    ctimer( &elapsed, &ucpu, &scpu );
    // print( "Cseq", m, n, Cseq );
    printf("Tiempo = %.1f segundos de la multiplicación simple\n",elapsed);
  }

  /*************************************************************************/
  /* # Comienzo del código a optimizar #                                   */
  /*************************************************************************/
  double elapsed, ucpu, scpu;
  ctimer( &elapsed, &ucpu, &scpu );

  double *Ac = (double *) malloc(mc * kc * sizeof(double)); 
  double *Bc = (double *) malloc(kc * nc * sizeof(double));//empaquetado

  for( int jc = 0; jc < n; jc += nc ) { // Bucle L1 (pasos de nc) 
    for( int pc = 0; pc < k; pc += kc ) { // Bucle L2 (pasos de kc)
      pack_B(kc, nc, &B[pc + jc * k], k, Bc); 
      for( int ic = 0; ic < m; ic += mc ) { // Bucle L3 (pasos de mc) 
        pack_A(mc, kc, &A[ic + pc * m], m, Ac);
        // multiplicacion usando buffers  
        for( int p = pc; p < pc + kc; p++ ) { 
          int pl = p - pc;
          for( int i = ic; i < ic + mc; i++ ) { 
            int il = i - ic;
            for( int j = jc; j < jc + nc; j++ ) { 
              int jl = j - jc;
              C[(i) + (j) * m] += Ac[il + pl * mc] * Bc[pl + jl * kc];
            } 
          } 
        } 
      } 
    } 
  }
  free(Ac); 
  free(Bc);

  ctimer( &elapsed, &ucpu, &scpu );
  // print( "C", m, n, C );
  printf("Tiempo = %.1f segundos de la multiplicación optimizada\n",elapsed);
  /*************************************************************************/
  /* # Fin del código a optimizar #                                        */
  /*************************************************************************/

  if( seq ) {
    /* Comprobación del resultado */
    /* Si la optimización es correcta el error debe ser pequeño o cero */
    double error = 0.0;
    for( int i = 0; i<m*n; i++ ) {
      error += fabs(C[i]-Cseq[i]);
    }
    printf("Error = %.2e\n",error);
  }
  
  free(A);
  free(B);
  free(Cseq);
  free(C);

  return 0;
}
