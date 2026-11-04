#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=2){
        if(rank==0){
            printf("Run with 2 process");
        }
        MPI_Finalize();
        return 0;
    }
    int A[2][2]={
        {1,2},
        {3,4}
    };
    int B[2][2]={
        {5,6},
        {7,8}
    };
    int row[2],result[2],C[2][2];
    if(rank==0){
        for(int j=0;j<2;j++)
        C[0][j]=A[0][0]*B[0][j]+A[0][1]*B[1][j];

        MPI_Send(A[1],2,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(C[1],2,MPI_INT,1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }else{
        MPI_Recv(row,2,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int j=0;j<2;j++)
        result[j]=row[0]*B[0][j]+row[1]*B[1][j];

        MPI_Send(result,2,MPI_INT,0,1,MPI_COMM_WORLD);
    }
    if(rank==0){
        printf("\nMatrix C=AxB\n");
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                printf("%d ",C[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}