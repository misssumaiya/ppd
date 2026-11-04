
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int send_data, recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with 2 processes\n");

        MPI_Finalize();
        return 0;
    }

    // Assign data
    if (rank == 0)
        send_data = 100;
    else
        send_data = 200;

    // Unsafe version (simulation)
    if (rank == 0) {
        printf("Unsafe version:\n");
        printf("P0: Waiting to send 100\n");
        printf("P1: Waiting to send 200\n");
        printf("Potential deadlock detected.\n");

        printf("\nCorrected version:\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Corrected version
    MPI_Sendrecv(&send_data, 1, MPI_INT, 1-rank, 0,
                 &recv_data, 1, MPI_INT, 1-rank, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Only P0 prints to maintain output order
    if (rank == 0) {
        printf("P0 sent 100\n");
        printf("P1 received %d\n", 100);
        printf("P1 sent 200\n");
        printf("P0 received %d\n", recv_data);
        printf("Communication completed without deadlock.\n");
    }
    MPI_Finalize();
    return 0;
}
