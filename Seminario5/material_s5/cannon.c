#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    double t_start = MPI_Wtime();
    const int ndims = 2;
    int p, rank, cart_rank;
    MPI_Comm comm2D;
    int dims[2] = {4, 4};
    int coord[2];
    int wrap_around[2] = {1, 1};
    int reorder = 1;
    int A, B, C = 0;
    int src, dst;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (p != 16) {
        if (rank == 0) printf("Se requieren exactamente 16 procesos\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, wrap_around, reorder, &comm2D);
    MPI_Comm_rank(comm2D, &cart_rank);
    MPI_Cart_coords(comm2D, cart_rank, ndims, coord);

    int i = coord[0];
    int j = coord[1];
    A = 4 * i + j + 1;
    B = 4 * i + j;

    int src_shift, dst_shift;

    MPI_Cart_shift(comm2D, 1, i, &src_shift, &dst_shift);
    MPI_Sendrecv_replace(&A, 1, MPI_INT, dst_shift, 0, src_shift, 0, comm2D, MPI_STATUS_IGNORE);

    MPI_Cart_shift(comm2D, 0, j, &src_shift, &dst_shift);
    MPI_Sendrecv_replace(&B, 1, MPI_INT, dst_shift, 0, src_shift, 0, comm2D, MPI_STATUS_IGNORE);

    for (int k = 0; k < 4; k++) {
        C += A * B;
        MPI_Cart_shift(comm2D, 1, 1, &src_shift, &dst_shift);
        MPI_Sendrecv_replace(&A, 1, MPI_INT, dst_shift, 0, src_shift, 0, comm2D, MPI_STATUS_IGNORE);
        MPI_Cart_shift(comm2D, 0, 1, &src_shift, &dst_shift);
        MPI_Sendrecv_replace(&B, 1, MPI_INT, dst_shift, 0, src_shift, 0, comm2D, MPI_STATUS_IGNORE);
    }

    if (rank == 0) {
        int C_matrix[4][4];
        C_matrix[i][j] = C;
        for (int r = 1; r < 16; r++) {
            int recv_C, recv_coord[2];
            MPI_Recv(&recv_C, 1, MPI_INT, r, 0, comm2D, MPI_STATUS_IGNORE);
            MPI_Cart_coords(comm2D, r, ndims, recv_coord);
            C_matrix[recv_coord[0]][recv_coord[1]] = recv_C;
        }
        printf("Matriz resultado C:\n");
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                printf("%5d ", C_matrix[x][y]);
            }
            printf("\n");
        }
    } else {
        MPI_Send(&C, 1, MPI_INT, 0, 0, comm2D);
    }

    MPI_Comm_free(&comm2D);

    double t_end = MPI_Wtime();
    if (rank == 0) printf("Tiempo total = %.4f s\n", t_end - t_start);
    MPI_Finalize();
    return 0;
}