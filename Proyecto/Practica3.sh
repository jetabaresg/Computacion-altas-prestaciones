#!/bin/bash
#SBATCH --job-name=practica3_summa
#SBATCH --nodes=4
#SBATCH --ntasks=16
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=8
#SBATCH --time=00:10:00
#SBATCH --partition=cna
#SBATCH --output=salida_p3_%j.out

mkdir -p ~/ejer_p3
cd ~/ejer_p3

mpicc -O3 -fopenmp -Wall -o practica3_summa $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica3_summa.c -lm

echo "PRÁCTICA SUMMA 2x2"
export OMP_NUM_THREADS=32
mpiexec -n 4 ./practica3_summa 1024
mpiexec -n 4 ./practica3_summa 2048
mpiexec -n 4 ./practica3_summa 4096

echo "PRUEBAS SUMMA 4x4"
export OMP_NUM_THREADS=8
mpiexec -n 16 ./practica3_summa 1024
mpiexec -n 16 ./practica3_summa 2048
mpiexec -n 16 ./practica3_summa 4096