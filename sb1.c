
#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    int a[12]={12,45,23,67,34,89,56,78,91,25,43,60};
    int local[4],max,i,result[3];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=4){
        if(rank==0)
            printf("Use 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    if(rank==0){
        printf("P0 distributed array elements to workers.\n");

        for(i=1;i<4;i++)
            MPI_Send(&a[(i-1)*4],4,MPI_INT,i,1,MPI_COMM_WORLD);

        for(i=1;i<4;i++){
            MPI_Recv(&result[i-1],1,MPI_INT,i,2,
                     MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("P%d found maximum: %d\n",i,result[i-1]);
        }

        for(i=1;i<4;i++)
            printf("P%d sent maximum: %d to P0\n",i,result[i-1]);

        max=result[0];
        for(i=1;i<3;i++)
            if(result[i]>max)
                max=result[i];

        printf("P0 received maximum values from all workers.\n");
        printf("Overall maximum element = %d\n",max);
    }
    else{
        MPI_Recv(local,4,MPI_INT,0,1,
                 MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        max=local[0];
        for(i=1;i<4;i++)
            if(local[i]>max)
                max=local[i];

        MPI_Send(&max,1,MPI_INT,0,2,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
