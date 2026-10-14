#!/bin/bash 
#SBATCH --job-name=practica1\_hpc 
#SBATCH --nodes=2 
#SBATCH --ntasks=4 
#SBATCH --time=00:10:00 
#SBATCH --partition=cna 
#SBATCH --output=/home/alumno/W/cna/Computacion-altas-prestaciones/Proyecto/salida\_p1\_%j.out 

# 1\. Crear directorio de trabajo en el disco local del nodo 
mkdir -p \~/ejer\_p1 
cd \~/ejer\_p1 
# 2\. Compilación del código MPI 
mpicc -O3 -Wall -o practica1\_matmul\_mpi $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica1\_matmul\_mpi.c -lm 
# 3\. Pruebas de ejecución 
mpiexec -n 2 ./practica1\_matmul\_mpi 1024 check 
mpiexec -n 2 ./practica1\_matmul\_mpi 2048 check 
mpiexec -n 4 ./practica1\_matmul\_mpi 1024 check 
mpiexec -n 4 ./practica1\_matmul\_mpi 2048 check 
mpiexec -n 4 ./practica1\_matmul\_mpi 4096 check