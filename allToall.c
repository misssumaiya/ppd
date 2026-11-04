#include<stdio.h>
#include<mpi.h>

int main(int argc, char**argv){
    int rank,size;
    int send[16],recv[16];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=4){
        if(rank==0){
            printf("Run with 4 process");
        }
        MPI_Finalize();
        return 0;
    }
    for(int i=0;i<size;i++){
        send[i]=rank*10+i;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double start=MPI_Wtime();
    MPI_Alltoall(send,1,MPI_INT,recv,1,MPI_INT,MPI_COMM_WORLD);
    double end=MPI_Wtime()-start;
    printf("P%d received: ",rank);
    for(int i=0;i<size;i++){
        printf("%d ",recv[i]);
    }
    printf("\n");
    printf("P%d time =%f s\n",rank,end);

    MPI_Finalize();
    return 0;
}