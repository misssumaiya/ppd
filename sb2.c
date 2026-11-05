
#include<stdio.h>
#include<mpi.h>

int main(int argc,char**argv){
    int rank,size;
    int a[5]={45,30,50,20,35};
    int local,prefix,manual,prev;
    int scan[5],result[5],values[5];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size!=5){
        if(rank==0)
            printf("Use 5 processes\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Scatter(a,1,MPI_INT,&local,1,MPI_INT,0,
                MPI_COMM_WORLD);

    if(rank==0)
        printf("P0 distributed elements to workers.\n");

    // Using MPI_Scan
    MPI_Scan(&local,&prefix,1,MPI_INT,MPI_MIN,
             MPI_COMM_WORLD);

    // Manual prefix minimum
    if(rank==0)
        manual=local;
    else{
        MPI_Recv(&prev,1,MPI_INT,rank-1,1,
                 MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        manual=(local<prev)?local:prev;
    }

    if(rank<size-1)
        MPI_Send(&manual,1,MPI_INT,rank+1,1,
                 MPI_COMM_WORLD);

    MPI_Gather(&local,1,MPI_INT,values,1,MPI_INT,
               0,MPI_COMM_WORLD);

    MPI_Gather(&prefix,1,MPI_INT,scan,1,MPI_INT,
               0,MPI_COMM_WORLD);

    MPI_Gather(&manual,1,MPI_INT,result,1,MPI_INT,
               0,MPI_COMM_WORLD);

    if(rank==0){
        for(int i=0;i<size;i++)
            printf("P%d: Local value = %d, Prefix minimum = %d\n",
                   i,values[i],scan[i]);

        printf("\nMPI_Scan result: ");
        for(int i=0;i<size;i++)
            printf("%d ",scan[i]);

        printf("\nManual result:   ");
        for(int i=0;i<size;i++)
            printf("%d ",result[i]);

        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
