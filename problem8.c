#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size,send_data,recv_data;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=2){
        if(rank==0){
            printf("Run with 2 processes\n");
        }
        MPI_Finalize();
        return 0;
    }
    //unsafe version
    if(rank==0)
    printf("Unsafe version\n");

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){
        send_data=100;
        printf("P0: waiting to send 100\n");
    }else{
        send_data=200;
        printf("p1: waiting to send 200\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0){
        printf("Potential deadlock detected.\n");
        printf("\nCorrected version\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    //corrected version
    MPI_Sendrecv(&send_data,1,MPI_INT,1-rank,0,&recv_data,1,MPI_INT,1-rank,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    if(rank==0){
        printf("P0 sent 100\n");
        printf("P0 recieved %d\n",recv_data);
    }else{
        printf("P1 received %d\n",recv_data);
        printf("p1 sent 200\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank==0){
        printf("communication completed without deadlock\n");
    }
    MPI_Finalize();
    return 0;

}