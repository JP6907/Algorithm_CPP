//
// Created by zjp on 18-12-5.
//
/**
 * 分发向量
 */
#include <stdio.h>
#include <mpi.h>

#define DATASIZE 100

void Scatter_Vector(int my_rank,int comm_sz,MPI_Comm comm);

int main(){
    int my_rank;
    int comm_sz;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    Scatter_Vector(my_rank,comm_sz,MPI_COMM_WORLD);

    MPI_Finalize();

}

void Scatter_Vector(int my_rank,int comm_sz,MPI_Comm comm){

    double a[DATASIZE];
    double local_a[DATASIZE];
    int local_n = DATASIZE/comm_sz;
    int i;

    if(my_rank==0) {
        for (i = 0; i < DATASIZE; i++)
            a[i] = i + 1;
        //数据分发，按自动按块划分
        MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
    }else{
        MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
        for(i=0;i<local_n;i++)
            printf("%d",local_a[i]);
        printf("\n");
    }
}