#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    const int ndims = 2;
    int p, rank, cart_rank;
    MPI_Comm comm2D;
    int dims[2], coord[2];
    int wrap_around[2];
    int reorder, nrows, ncols;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 3) {
        if (rank == 0) printf("Usage: mpirun -np %d %s nrows ncols\n", p, argv[0]);
        MPI_Finalize();
        return 0;
    }
    sscanf(argv[1], "%d", &nrows);
    sscanf(argv[2], "%d", &ncols);

    if (nrows * ncols > p) {
        if (rank == 0) printf("%d debe ser >= %d\n", p, nrows * ncols);
        MPI_Finalize();
        return 0;
    }

    dims[0] = nrows;
    dims[1] = ncols;
    wrap_around[0] = 0;
    wrap_around[1] = 0;
    reorder = 1;

    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, wrap_around, reorder, &comm2D);

    if (comm2D != MPI_COMM_NULL) {
        MPI_Comm_rank(comm2D, &cart_rank);
        MPI_Cart_coords(comm2D, cart_rank, ndims, coord);
        printf("Rank %d: coord (%d, %d)\n", rank, coord[0], coord[1]);

        MPI_Comm row_comm, col_comm;
        int remain_dims[2];

        remain_dims[0] = 0;
        remain_dims[1] = 1;
        MPI_Cart_sub(comm2D, remain_dims, &row_comm);

        remain_dims[0] = 1;
        remain_dims[1] = 0;
        MPI_Cart_sub(comm2D, remain_dims, &col_comm);

        int row_rank = -1, col_rank = -1;
        if (row_comm != MPI_COMM_NULL) MPI_Comm_rank(row_comm, &row_rank);
        if (col_comm != MPI_COMM_NULL) MPI_Comm_rank(col_comm, &col_rank);

        printf("Rank %d: row_rank=%d, col_rank=%d\n", rank, row_rank, col_rank);

        if (row_comm != MPI_COMM_NULL) MPI_Comm_free(&row_comm);
        if (col_comm != MPI_COMM_NULL) MPI_Comm_free(&col_comm);
        MPI_Comm_free(&comm2D);
    }

    MPI_Finalize();
    return 0;
}