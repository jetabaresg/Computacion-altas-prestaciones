#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int p, rank;
    MPI_Group group_world;
    MPI_Group comm1_group, comm2_group;
    MPI_Comm comm1, comm2;
    int *comm1_procs, *comm2_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (p < 2) {
        if (rank == 0) printf("ERROR: The number of processes %d must be >= 2\n", p);
        MPI_Finalize();
        return 0;
    }

    comm1_procs = malloc(p * sizeof(int));
    comm2_procs = malloc(p * sizeof(int));
    int c1 = 0, c2 = 0;
    for (int i = 0; i < p; i++) {
        if (i % 2 == 0) comm1_procs[c1++] = i;
        else comm2_procs[c2++] = i;
    }

    MPI_Comm_group(MPI_COMM_WORLD, &group_world);
    MPI_Group_incl(group_world, c1, comm1_procs, &comm1_group);
    MPI_Group_incl(group_world, c2, comm2_procs, &comm2_group);

    MPI_Comm_create(MPI_COMM_WORLD, comm1_group, &comm1);
    MPI_Comm_create(MPI_COMM_WORLD, comm2_group, &comm2);

    int guarda = (rank % 2 == 0);
    MPI_Comm comm = guarda ? comm1 : comm2;

    int comm_rank = -1;
    if (comm != MPI_COMM_NULL) {
        MPI_Comm_rank(comm, &comm_rank);
        int val = 1, sum = 0;
        MPI_Bcast(&val, 1, MPI_INT, 0, comm);
        MPI_Reduce(&val, &sum, 1, MPI_INT, MPI_SUM, 0, comm);
        if (comm_rank == 0) {
            int size;
            MPI_Comm_size(comm, &size);
            printf("Grupo %d: suma = %d, tamano = %d\n", guarda + 1, sum, size);
        }
    }

    comm = guarda ? comm1 : comm2;
    if (comm != MPI_COMM_NULL) {
        MPI_Comm_rank(comm, &comm_rank);
        int val = 1, sum = 0;
        MPI_Bcast(&val, 1, MPI_INT, 0, comm);
        MPI_Reduce(&val, &sum, 1, MPI_INT, MPI_SUM, 0, comm);
        if (comm_rank == 0) {
            int size;
            MPI_Comm_size(comm, &size);
            printf("Grupo %d (refactor): suma = %d, tamano = %d\n", guarda + 1, sum, size);
        }
    }

    if (comm1 != MPI_COMM_NULL) MPI_Comm_free(&comm1);
    if (comm2 != MPI_COMM_NULL) MPI_Comm_free(&comm2);
    MPI_Group_free(&comm1_group);
    MPI_Group_free(&comm2_group);
    MPI_Group_free(&group_world);
    free(comm1_procs);
    free(comm2_procs);

    MPI_Finalize();
    return 0;
}