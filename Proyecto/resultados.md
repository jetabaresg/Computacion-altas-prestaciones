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


jtabgal@alumno.upv.es@kahan:~/ejer_p4$ cat salida_p4_*.out
PRÁCTICA 4: ALGORITMO SUMMA + BLOQUES BLIS
MALLA 2x2: 4 Procesos MPI (1 por nodo) x 32 Hilos
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 0.4393 s | Rendimiento: 4.89 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0623 segundos
 - Rendimiento: 34.49 GFLOPS
 - Speedup (S): 7.0554
 - Error (norma euclídea): 5.448e+05
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 4.5005 s | Rendimiento: 3.82 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3451 segundos
 - Rendimiento: 49.78 GFLOPS
 - Speedup (S): 13.0393
 - Error (norma euclídea): 1.758e+06
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 2048
Resultado Secuencial:
 - Tiempo: 36.3623 s | Rendimiento: 3.78 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.1763 segundos
 - Rendimiento: 63.15 GFLOPS
 - Speedup (S): 16.7084
 - Error (norma euclídea): 5.282e+06
MALLA 4x4: 16 Procesos MPI (4 por nodo) x 8 Hilos
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 256
Resultado Secuencial:
 - Tiempo: 0.4981 s | Rendimiento: 4.31 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0570 segundos
 - Rendimiento: 37.70 GFLOPS
 - Speedup (S): 8.7442
 - Error (norma euclídea): 3.627e+05
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 4.7929 s | Rendimiento: 3.58 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3268 segundos
 - Rendimiento: 52.58 GFLOPS
 - Speedup (S): 14.6678
 - Error (norma euclídea): 2.123e+06
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 38.9656 s | Rendimiento: 3.53 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.2272 segundos
 - Rendimiento: 61.71 GFLOPS
 - Speedup (S): 17.4953
 - Error (norma euclídea): 6.116e+06
PRÁCTICA 4: ALGORITMO SUMMA + BLOQUES BLIS
MALLA 2x2: 4 Procesos MPI (1 por nodo) x 32 Hilos
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 0.4420 s | Rendimiento: 4.86 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0652 segundos
 - Rendimiento: 32.95 GFLOPS
 - Speedup (S): 6.7816
 - Error (norma euclídea): 6.111e+05
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 4.4921 s | Rendimiento: 3.82 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3434 segundos
 - Rendimiento: 50.02 GFLOPS
 - Speedup (S): 13.0797
 - Error (norma euclídea): 1.710e+06
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 2048
Resultado Secuencial:
 - Tiempo: 36.4768 s | Rendimiento: 3.77 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.1880 segundos
 - Rendimiento: 62.82 GFLOPS
 - Speedup (S): 16.6714
 - Error (norma euclídea): 4.802e+06
MALLA 4x4: 16 Procesos MPI (4 por nodo) x 8 Hilos
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 256
Resultado Secuencial:
 - Tiempo: 0.4567 s | Rendimiento: 4.70 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0563 segundos
 - Rendimiento: 38.15 GFLOPS
 - Speedup (S): 8.1142
 - Error (norma euclídea): 2.632e+05
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 4.6402 s | Rendimiento: 3.70 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3363 segundos
 - Rendimiento: 51.09 GFLOPS
 - Speedup (S): 13.7991
 - Error (norma euclídea): 2.096e+06
=== PRÁCTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 38.9494 s | Rendimiento: 3.53 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.1804 segundos
 - Rendimiento: 63.03 GFLOPS
 - Speedup (S): 17.8635
 - Error (norma euclídea): 6.038e+06
PRÁCTICA 4: ALGORITMO SUMMA + BLOQUES BLIS
MALLA 2x2: 4 Procesos MPI (1 por nodo) x 32 Hilos
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 0.4436 s | Rendimiento: 4.84 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0624 segundos
 - Rendimiento: 34.44 GFLOPS
 - Speedup (S): 7.1146
 - Error (norma euclidea): 6.209e+05
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 4.5001 s | Rendimiento: 3.82 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3425 segundos
 - Rendimiento: 50.17 GFLOPS
 - Speedup (S): 13.1407
 - Error (norma euclidea): 1.752e+06
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 2048
Resultado Secuencial:
 - Tiempo: 36.4429 s | Rendimiento: 3.77 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.2061 segundos
 - Rendimiento: 62.30 GFLOPS
 - Speedup (S): 16.5190
 - Error (norma euclidea): 5.176e+06
MALLA 4x4: 16 Procesos MPI (4 por nodo) x 8 Hilos
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 1024 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 256
Resultado Secuencial:
 - Tiempo: 0.4797 s | Rendimiento: 4.48 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.0638 segundos
 - Rendimiento: 33.68 GFLOPS
 - Speedup (S): 7.5223
 - Error (norma euclidea): 4.005e+05
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 2048 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 4.8584 s | Rendimiento: 3.54 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 0.3369 segundos
 - Rendimiento: 51.00 GFLOPS
 - Speedup (S): 14.4211
 - Error (norma euclidea): 2.147e+06
=== PRACTICA 4: SUMMA CON BLOQUES Y EMPAQUETAMIENTO ===
Matriz N = 4096 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 38.9616 s | Rendimiento: 3.53 GFLOPS

Resultado Paralelo (SUMMA + BLIS):
 - Tiempo (Tpar): 2.2101 segundos
 - Rendimiento: 62.19 GFLOPS
 - Speedup (S): 17.6287
 - Error (norma euclidea): 6.070e+06
jtabgal@alumno.upv.es@kahan:~/ejer_p4$ 

//////////////////////////////////


jtabgal@alumno.upv.es@kahan:~/ejer_p5$ cat salida_p5_*.out
PRÁCTICA 5
MALLA 2x2: 4 Procesos MPI (1 por nodo) x 32 Hilos
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 1024 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 0.3941 s | Rendimiento: 5.45 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 0.0489 segundos
 - Rendimiento: 43.89 GFLOPS
 - Speedup (S): 8.0560
 - Error (norma euclidea): 5.965e+05
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 2048 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 4.1265 s | Rendimiento: 4.16 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 0.2686 segundos
 - Rendimiento: 63.96 GFLOPS
 - Speedup (S): 15.3621
 - Error (norma euclidea): 1.803e+06
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 4096 | Malla 2D = 2x2 (4 procesos) | Bloque local b = 2048
Resultado Secuencial:
 - Tiempo: 33.3466 s | Rendimiento: 4.12 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 1.6898 segundos
 - Rendimiento: 81.33 GFLOPS
 - Speedup (S): 19.7339
 - Error (norma euclidea): 5.290e+06
MALLA 4x4: 16 Procesos MPI (4 por nodo) x 8 Hilos
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 1024 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 256
Resultado Secuencial:
 - Tiempo: 0.4243 s | Rendimiento: 5.06 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 0.0442 segundos
 - Rendimiento: 48.62 GFLOPS
 - Speedup (S): 9.6057
 - Error (norma euclidea): 2.815e+05
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 2048 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 512
Resultado Secuencial:
 - Tiempo: 4.0951 s | Rendimiento: 4.20 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 0.2214 segundos
 - Rendimiento: 77.60 GFLOPS
 - Speedup (S): 18.4976
 - Error (norma euclidea): 2.174e+06
=== PRACTICA 5: SUMMA + BLOQUES + MICRO-KERNEL SIMD (AVX/FMA) ===
Matriz N = 4096 | Malla 2D = 4x4 (16 procesos) | Bloque local b = 1024
Resultado Secuencial:
 - Tiempo: 35.0828 s | Rendimiento: 3.92 GFLOPS

Resultado Paralelo (SUMMA + Blocking + SIMD AVX/FMA):
 - Tiempo (Tpar): 1.2899 segundos
 - Rendimiento: 106.55 GFLOPS
 - Speedup (S): 27.1979
 - Error (norma euclidea): 6.041e+06
