jtabgal@alumno.upv.es@kahan:~/ejer_p1$ cat salida_p1*.out
 PRUEBAS PRÁCTICA 1
--- N = 1024 (4 procesos) ---
Practica 1 
Proceso P0 ejecutando con N = 1024 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 0.4427 segundos
 - Rendimiento: 4.85 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.1538 segundos
 - Rendimiento: 13.96 GFLOPS
 - Speedup (S): 2.8783
 - Eficiencia (E): 0.7196
 - Error (norma euclidea C - Cseq): 0.000e+00
--- N = 2048 (4 procesos) ---
Practica 1 
Proceso P0 ejecutando con N = 2048 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 4.4962 segundos
 - Rendimiento: 3.82 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 1.2647 segundos
 - Rendimiento: 13.58 GFLOPS
 - Speedup (S): 3.5551
 - Eficiencia (E): 0.8888
 - Error (norma euclidea C - Cseq): 0.000e+00
--- N = 4096 (4 procesos) ---
Practica 1 
Proceso P0 ejecutando con N = 4096 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 36.4643 segundos
 - Rendimiento: 3.77 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 9.5811 segundos
 - Rendimiento: 14.34 GFLOPS
 - Speedup (S): 3.8059
 - Eficiencia (E): 0.9515
 - Error (norma euclidea C - Cseq): 0.000e+00

/////////////////////////////////

tabgal@alumno.upv.es@kahan:~/ejer_p2$ cat salida_p2_*out
PRÁCTICA 2 (MPI + OpenMP)
--- CONFIGURACIÓN 1: 4 MPI (1 por nodo) x 32 Hilos ---
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 1024 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 0.0939 segundos
 - Rendimiento: 22.87 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.0780 segundos
 - Rendimiento: 27.52 GFLOPS
 - Speedup (S): 1.2033
 - Eficiencia (E): 0.3008
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 2048 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 1.5940 segundos
 - Rendimiento: 10.78 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.6188 segundos
 - Rendimiento: 27.76 GFLOPS
 - Speedup (S): 2.5758
 - Eficiencia (E): 0.6440
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 4096 y 4 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 15.4961 segundos
 - Rendimiento: 8.87 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 5.9296 segundos
 - Rendimiento: 23.18 GFLOPS
 - Speedup (S): 2.6133
 - Eficiencia (E): 0.6533
 - Error (norma euclidea C - Cseq): 0.000e+00
CONFIGURACIÓN 2: 8 MPI (2 por nodo) x 16 Hilos
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 1024 y 8 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 0.0887 segundos
 - Rendimiento: 24.22 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.0798 segundos
 - Rendimiento: 26.90 GFLOPS
 - Speedup (S): 1.1107
 - Eficiencia (E): 0.1388
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 2048 y 8 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 0.9156 segundos
 - Rendimiento: 18.76 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.8103 segundos
 - Rendimiento: 21.20 GFLOPS
 - Speedup (S): 1.1299
 - Eficiencia (E): 0.1412
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 4096 y 8 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 11.3019 segundos
 - Rendimiento: 12.16 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 7.6514 segundos
 - Rendimiento: 17.96 GFLOPS
 - Speedup (S): 1.4771
 - Eficiencia (E): 0.1846
 - Error (norma euclidea C - Cseq): 0.000e+00
CONFIGURACIÓN 3: 16 MPI (4 por nodo) x 8 Hilos 
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 1024 y 16 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 0.1315 segundos
 - Rendimiento: 16.33 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 0.0966 segundos
 - Rendimiento: 22.23 GFLOPS
 - Speedup (S): 1.3618
 - Eficiencia (E): 0.0851
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 2048 y 16 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 1.0054 segundos
 - Rendimiento: 17.09 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 1.0115 segundos
 - Rendimiento: 16.98 GFLOPS
 - Speedup (S): 0.9939
 - Eficiencia (E): 0.0621
 - Error (norma euclidea C - Cseq): 0.000e+00
=== PRACTICA 1 ===
Proceso P0 ejecutando con N = 4096 y 16 procesos MPI iniciados.
Resultado Secuencial:
 - Tiempo (Tseq): 8.4176 segundos
 - Rendimiento: 16.33 GFLOPS

Resultado Paralelo (MPI):
 - Tiempo (Tpar): 8.3601 segundos
 - Rendimiento: 16.44 GFLOPS
 - Speedup (S): 1.0069
 - Eficiencia (E): 0.0629
 - Error (norma euclidea C - Cseq): 0.000e+00


////////////////////////////////////////

PRÁCTICA 3: ALGORITMO SUMMA (MALLA 2D)
 MALLA 2x2: 4 MPI (1 por nodo) x 32 Hilos
SUMMA (Malla 2x2) - N=1024 - Tiempo: 0.0501 s - GFLOPS: 42.89
SUMMA (Malla 2x2) - N=2048 - Tiempo: 0.2285 s - GFLOPS: 75.20
SUMMA (Malla 2x2) - N=4096 - Tiempo: 3.5585 s - GFLOPS: 38.62
--- MALLA 4x4: 16 MPI (4 por nodo) x 8 Hilos ---
SUMMA (Malla 4x4) - N=1024 - Tiempo: 0.0451 s - GFLOPS: 47.64
SUMMA (Malla 4x4) - N=2048 - Tiempo: 0.2029 s - GFLOPS: 84.68
SUMMA (Malla 4x4) - N=4096 - Tiempo: 4.7379 s - GFLOPS: 29.01


//////////////////////////////////////


