#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=10:00
#SBATCH --partition=cna
export OMP_NUM_THREADS=1
./matrixproduct 512