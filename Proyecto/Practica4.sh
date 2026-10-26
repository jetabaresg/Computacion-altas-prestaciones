#!/bin/bash
#SBATCH --job-name=practica4_blocking
#SBATCH --nodes=4
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --partition=cna
#SBATCH --output=salida_p4_%j.out

mkdir -p ~/ejer_p4
cd ~/ejer_p4

# Compilación con soporte OpenMP y optimizaciones del compilador
mpicc -O3 -fopenmp -Wall -o practica4_blocking $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica4_summa_blocking.c -lm

echo "=== PRUEBAS PRÁCTICA 4 (SUMMA + BLOQUEO) ==="
export OMP_NUM_THREADS=8

mpiexec -n 16 ./practica4_blocking 1024
mpiexec -n 16 ./practica4_blocking 2048
mpiexec -n 16 ./practica4_blocking 4096