#!/bin/bash 
#SBATCH --job-name=practica1_hpc 
#SBATCH --nodes=2 
#SBATCH --ntasks=4 
#SBATCH --time=00:10:00 
#SBATCH --partition=cna 
#SBATCH --output=salida_p1_%j.out

# 1. Crear directorio de trabajo en el disco local del nodo 
mkdir -p ~/ejer_p1 
cd ~/ejer_p1 
# 2. Compilación del código MPI 
mpicc -O3 -Wall -o practica1_matmul_mpi $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica1_matmul_mpi.c -lm 
# 3. Pruebas de ejecución 
mpiexec -n 2 ./practica1_matmul_mpi 1024 check 
mpiexec -n 2 ./practica1_matmul_mpi 2048 check 
mpiexec -n 4 ./practica1_matmul_mpi 1024 check 
mpiexec -n 4 ./practica1_matmul_mpi 2048 check 
mpiexec -n 4 ./practica1_matmul_mpi 4096 check