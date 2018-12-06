//
// Created by zjp on 18-12-3.
//
/**
 * 将一组数相加
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define MAXSIZE 1000

int main(int argc,char **argv){
    int myrank,numprocs;
    int data[MAXSIZE],i,x,low,high,myresult,result;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    //在进程0初始化数据
    if(myrank==0){
        for(i=0;i<MAXSIZE;i++)
            data[i] = i+1;
    }

    //广播数据
    MPI_Bcast(data,MAXSIZE,MPI_INT,0,MPI_COMM_WORLD);
    //分片计算
    x = MAXSIZE/numprocs;
    low = myrank*x;
    high = low+x;
    myresult = 0;
    for(i=low;i<high;i++)
        myresult += data[i];
    printf("I got %d from %d\n",myresult,myrank);
    //计算总和
    MPI_Reduce(&myresult,&result,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(myrank==0)
        printf("The sum is %d\n",result);
}
