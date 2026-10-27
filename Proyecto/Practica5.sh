#!/bin/bash
#SBATCH --job-name=practica5_simd
#SBATCH --nodes=4
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --partition=cna
#SBATCH --output=salida_p4_%j.out

mkdir -p ~/ejer_p5
cd ~/ejer_p5

# Compilación con soporte OpenMP, vectorización nativa y FMA
mpicc -O3 -fopenmp -march=native -mfma -Wall -o practica5_simd $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica5_summa_simd.c -lm

echo "PRUEBAS PRÁCTICA 5 SUMMA + BLOQUEO + SIMD"
export OMP_NUM_THREADS=8

mpiexec -n 16 ./practica5_simd 1024
mpiexec -n 16 ./practica5_simd 2048
mpiexec -n 16 ./practica5_simd 4096