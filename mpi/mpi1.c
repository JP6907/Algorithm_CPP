//
// Created by zjp on 18-12-3.
//
/**
 * 点对点消息接发
 * 进程0向进程1发送一个整数
*/

#include <stdio.h>
#include <mpi.h>

#define N 100000

int main(int argc,char **argv) {
    int myrank,i;
    int a[N];
    MPI_Status status;

    //init mpi environment
    MPI_Init(&argc,&argv);

    //init array
    for(i=0;i<N;i++){
        a[i] = -1;
    }

    //获取当前进程在通信器MPI_COMM_WORLD中的进程号
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    //让进程0，数组中的每一个元素都为100
    if(myrank==0){
        for(i=0;i<N;i++){
            a[i] = 100;
        }
        //进程0向进程1发送一个整数
        MPI_Send(&a,N,MPI_INT,1,99,MPI_COMM_WORLD);
    }else{
        fprintf(stderr,"myrank=%d,before recv. a[0]=%d\n",myrank,a[0]);
        //进程1从进程0接收一个整数
        //MPI_Recv语句多了一个status
        MPI_Recv(&a,N,MPI_INT,0,99,MPI_COMM_WORLD,&status);
        fprintf(stderr,"myrank=%d,after recv. a[0]=%d\n\n",myrank,a[0]);
    }
    //结束MPI环境，测试程序分配两个进程即可，多了其他进程陷入接收的等待中
    MPI_Finalize();

    return 0;
}
