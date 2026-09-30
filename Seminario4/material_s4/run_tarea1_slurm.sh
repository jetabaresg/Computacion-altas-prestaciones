```
#!/bin/bash
#SBATCH --job-name=s4_tarea1
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=15:00
#SBATCH --partition=cna
#SBATCH --output=/home/alumno/W/cna/Seminario4/salida_tarea1_%j.out

# Directa ejecución secuencial
export OMP_NUM_THREADS=1

# Crear carpeta local y compilar
mkdir -p ~/ejer_s4
cd ~/ejer_s4
gcc -Wall -fopenmp -o cholesky ~/W/cna/Seminario4/cholesky.c ~/W/cna/Seminario4/ctimer.c -llapack -lblas -lm

echo "========================================================="
echo "   TAREA 1: BÚSQUEDA DEL TAMAÑO DE BLOQUE ÓPTIMO (b)"
echo "========================================================="

for n in 1024 2048 4096; do
    echo "---------------------------------------------------------"
    echo " Pruebas con N = $n"
    echo "---------------------------------------------------------"
    for b in 16 32 64 128 256 512; do
        ./cholesky $n $b
    done
done