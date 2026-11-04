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
    int sample[4][4]={
        {1,2,2,3},
        {2,3,4,4},
        {1,4,5,5},
        {0,2,5,9}
    };
    int local[10]={0};
    int global[10];
    for(int i=0;i<4;i++){
        local[sample[rank][i]]++;
    }
    MPI_Allreduce(local,global,10,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    if(rank==0){
        printf("value  frequency: \n");
        for(int i=0;i<10;i++){
            printf("%d %d\n",i,global[i]);
        }
        printf("Total num of elements=16\n");
    }
    MPI_Finalize();
    return 0;
}