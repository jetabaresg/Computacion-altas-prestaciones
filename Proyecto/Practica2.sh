!/bin/bash
SBATCH --job-name=practica2\_hpc 
SBATCH --nodes=1 
SBATCH --ntasks=4 
SBATCH --cpus-per-task=4 
SBATCH --time=00:10:00 
SBATCH --partition=cna 
SBATCH --output=/home/alumno/W/cna/Computacion-altas-prestaciones/Proyecto/salida_p2\_%j.out 
# 1\. Crear directorio local 
mkdir -p \~/ejer\_p2 
cd \~/ejer\_p2 
# 2\. Compilación con soporte para hilos OpenMP (-fopenmp) 
mpicc -O3 -fopenmp -Wall -o practica2\_hibrido $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica2\_hibrido.c -lm 
# 3\. Evaluación de combinaciones fijando 4 núcleos en total 
echo "=== 4 Procesos MPI x 1 Hilo OpenMP ===" 
export OMP\_NUM\_THREADS=1 
mpiexec -n 4 ./practica2\_hibrido 2048 check 
echo "=== 2 Procesos MPI x 2 Hilos OpenMP ===" 
export OMP\_NUM\_THREADS=2 
mpiexec -n 2 ./practica2\_hibrido 2048 check 
echo "=== 1 Proceso MPI x 4 Hilos OpenMP ===" 
export OMP\_NUM\_THREADS=4 
mpiexec -n 1 ./practica2\_hibrido 2048 check