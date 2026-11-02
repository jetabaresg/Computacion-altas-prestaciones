#!/bin/bash
#SBATCH --job-name=practica5_simd
#SBATCH --nodes=4
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --partition=cna
#SBATCH --output=salida_p5_%j.out

cd $HOME/ejer_p5

echo "=== PRÁCTICA 5: SUMMA + BLOQUES + SIMD AVX/FMA ==="

echo "--- MALLA 2x2: 4 Procesos MPI (1 por nodo) x 32 Hilos ---"
export OMP_NUM_THREADS=32
mpiexec -n 4 --map-by ppr:1:node ./practica5_simd 1024 check
mpiexec -n 4 --map-by ppr:1:node ./practica5_simd 2048 check
mpiexec -n 4 --map-by ppr:1:node ./practica5_simd 4096 check

echo "--- MALLA 4x4: 16 Procesos MPI (4 por nodo) x 8 Hilos ---"
export OMP_NUM_THREADS=8
mpiexec -n 16 --map-by ppr:4:node ./practica5_simd 1024 check
mpiexec -n 16 --map-by ppr:4:node ./practica5_simd 2048 check
mpiexec -n 16 --map-by ppr:4:node ./practica5_simd 4096 check