
#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size,value,prefix[5],sum;

    int a[5]={45,30,50,20,35};

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=5){
        if(rank==0) printf("Run with 5 processes\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Scatter(a,1,MPI_INT,&value,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Scan(&value,&sum,1,MPI_INT,MPI_MIN,MPI_COMM_WORLD);

    printf("P%d: Local value=%d, Prefix minimum=%d\n",rank,value,sum);

    MPI_Gather(&sum,1,MPI_INT,prefix,1,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==0){
        printf("Prefix minimum result: ");
        for(int i=0;i<size;i++)
            printf("%d ",prefix[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
