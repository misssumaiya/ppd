#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    int a[8],b[8];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0){
        a[0]=10;
        a[1]=20;
        a[2]=30;
        a[3]=40;
        a[4]=50;
        a[5]=60;
        a[6]=70;
        a[7]=80;
    }else{
        a[0]=1;
        a[1]=2;
        a[2]=3;
        a[3]=4;
        a[4]=5;
        a[5]=6;
        a[6]=7;
        a[7]=8;
    }
    double start=MPI_Wtime();
    MPI_Sendrecv(a,8,MPI_INT,1-rank,0,
        b,8,MPI_INT,1-rank,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    double blocking_time=MPI_Wtime()-start;
    MPI_Barrier(MPI_COMM_WORLD);

    start=MPI_Wtime();
    MPI_Request req1,req2;

    MPI_Irecv(b,8,MPI_INT,1-rank,1,MPI_COMM_WORLD,&req1);
    MPI_Isend(a,8,MPI_INT,1-rank,1,MPI_COMM_WORLD,&req2);

    MPI_Wait(&req1,MPI_STATUS_IGNORE);
    MPI_Wait(&req2,MPI_STATUS_IGNORE);

    double nonBlocking_time=MPI_Wtime()-start;

    printf("Process %d received: ",rank);

    for(int i=0;i<8;i++){
        printf("%d ",b[i]);
    }
    if(rank==0){
        printf("\nBlocking time: %f seconds\n",blocking_time);
        printf("\nNon blocking time: %f seconds\n",nonBlocking_time);
    }
    MPI_Finalize();
    return 0;

}