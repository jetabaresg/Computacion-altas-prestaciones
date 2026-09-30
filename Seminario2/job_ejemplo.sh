#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=5:00
#SBATCH --partition=cna
export OMP_NUM_THREADS=4
./ejemplo