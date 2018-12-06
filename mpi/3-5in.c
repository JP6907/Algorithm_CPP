//
// Created by zjp on 18-12-5.
//
/**
 * 由一个进程负责接收输入
 */

#include <stdio.h>
#include <mpi.h>

void Get_input(int my_rank,int comm_sz,double* a,double* b,int* n);

int main(){
    int my_rank,comm_sz;
    double a,b;
    int n;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    Get_input(my_rank,comm_sz,&a,&b,&n);

    MPI_Finalize();
    return 0;
}

void Get_input(int my_rank,int comm_sz,double* a,double* b,int* n){
    int dest;

//    if(my_rank==0){
//        printf("Enter a,b,n:\n");
//        scanf("%lf %lf %d",a,b,n);
//        for(dest=1;dest<comm_sz;dest++){ //发送给其他进程
//            MPI_Send(a,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
//            MPI_Send(b,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
//            MPI_Send(n,1,MPI_INT,dest,0,MPI_COMM_WORLD);
//        }
//    }else{
//        MPI_Recv(a,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//        MPI_Recv(b,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//        MPI_Recv(n,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//        printf("process %d : a=%lf b=%lf n=%d\n",my_rank,*a,*b,*b);
//    }

    //使用bcast广播代替逐个send recv

    if(my_rank==0){
        printf("Enter a,b,n:\n");
        scanf("%lf %lf %d",a,b,n);
    }
    MPI_Bcast(a,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(b,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
}

