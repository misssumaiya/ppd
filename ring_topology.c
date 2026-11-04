#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
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
    int value=100,received;
    double start,end;

    int next=(rank+1)%size;
    int prev=(rank-1+size)%size;

    MPI_Barrier(MPI_COMM_WORLD);

    start=MPI_Wtime();
    MPI_Sendrecv(&value,1,MPI_INT,next,1,
    &received,1,MPI_INT,prev,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    end=MPI_Wtime();
    double elapsed=end-start,max_elapsed;

    printf("P%d send %d to p%d\n",rank,value,next);
    printf("P%d received %d from p%d\n",rank,received,prev);

    MPI_Reduce(&elapsed,&max_elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank==0){
        printf("Ring communication completed\n");
        printf("Maximum communication time=%.9f s \n",max_elapsed);
    }
    MPI_Finalize();
    return 0;
}