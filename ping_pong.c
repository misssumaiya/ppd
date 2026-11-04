#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

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
    int sizes[]={64,256,1024,4096,16384,65536,1048576};
    int iterations=5;
    for(int k=0; k<iterations;k++){
        int bytes=sizes[k];
        char*data=malloc(bytes);

        if(data==NULL){
            printf("Memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD,1);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        double start=MPI_Wtime();

        for(int i=0; i<iterations;i++){
            if(rank==0){
                MPI_Send(data,bytes,MPI_CHAR,1,0,MPI_COMM_WORLD);
                MPI_Recv(data,bytes,MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }else{
                MPI_Recv(data,bytes,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                MPI_Send(data,bytes,MPI_CHAR,0,0,MPI_COMM_WORLD);
            }
        }
        double end=MPI_Wtime();
        
        if(rank==0){
            double time=end-start;
            double roundTrip=time/iterations;
            double oneway=roundTrip/2.0;
            double bw=(2.0*bytes*iterations)/time/1000000.0;
            printf("Message sizes: %d bytes\n",bytes);
            printf("Iterations: %d\n",iterations);
            printf("Average roundTrip: %f\n",roundTrip);
            printf("Average oneway: %f\n",oneway);
            printf("Bandwidth: %f\n",bw);
        }
        free(data);
    }
    MPI_Finalize();
    return 0;

}