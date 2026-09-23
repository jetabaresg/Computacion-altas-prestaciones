#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=10:00
#SBATCH --partition=cna
export OMP_NUM_THREADS=8
./knapsackBF_omp 24