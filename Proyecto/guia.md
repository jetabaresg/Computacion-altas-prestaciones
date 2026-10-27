Crear archivos de compilacion:

mkdir -p ~/ejer_p1 cd ~/ejer_p

\# 2\. Compilar el codigo C:
 mpicc -O3 -Wall -o practica1\_matmul\_mpi $HOME/W/cna/Computacion-altas-prestaciones/Proyecto/Practica1\_matmul\_mpi.c -lm
  
 "--- 2 PROCESOS MPI ---" mpiexec -n 2 ./practica1\_matmul\_mpi 1024 check mpiexec -n 2 ./practica1\_matmul\_mpi 2048 check echo "--- 4 PROCESOS MPI ---" mpiexec -n 4 ./practica1\_matmul\_mpi 1024 check mpiexec -n 4 ./practica1\_matmul\_mpi 2048 check mpiexec -n 4 ./practica1\_matmul\_mpi 4096 check EOF

Mandar a la cola Kahan : 

sbatch ~/W/cna/Computacion-altas-prestaciones/Proyecto/Practica1.sh

Comprobar estado : 

squeue -u $USER