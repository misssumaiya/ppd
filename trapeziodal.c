#include<stdio.h>
#include<mpi.h>
#include<math.h>

static double f(double x){
    return x*x;
}

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
    double a=0.0,b=1.0;
    long n=10000;
    double h=(b-a)/n;
    double local=0.0,global=0.0;
    for(int i=rank; i<n; i+=size){
        double x0=a+i*h;
        double x1=x0+h;
        local+=(f(x0)+f(x1))*h/2.0;
    }
    printf("P%d partial integral = %f\n",rank,local);
    MPI_Reduce(&local,&global,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0){
        double exact=1.0/3.0;
        printf("Exact integral: %d\n",exact);
        printf("Numerical Integral: %f\n",global);
        double error=fabs(exact-global);
        printf("Error: %f\n",error);
    }
    MPI_Finalize();
    return 0;
}