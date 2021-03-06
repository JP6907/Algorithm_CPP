//
// Created by zjp on 18-12-9.
//
/**
 * 蝶形结构通信
 * 通信域comm_sz的进程数是2的幂的特殊情况
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(){
    int my_rank,comm_sz;
    int n =  1004,extend_n =0,local_n,i;
    int *data,*local_data;
    int src,dst;
    int sum=0,local_sum=0,recv_sum=0;
    int current_step_size;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    //如果n不能被进程数整除以，补充长度到可以被整除
    if(n%comm_sz!=0)
        extend_n = comm_sz*(n/comm_sz+1) - n;

    local_n = (n+extend_n)/comm_sz;
    data = (int*)malloc(sizeof(int)*(n+extend_n));
    local_data = (int*)malloc(sizeof(int)*local_n);

    if(my_rank==0){
        for(i=0;i<n;i++)
            data[i] = i;
    }

    //分发数据
    MPI_Scatter(data,local_n,MPI_INT,local_data,local_n,MPI_INT,0,MPI_COMM_WORLD);
    //计算local_sum
    for(i=0;i<local_n;i++)
        local_sum += local_data[i];

    //每组数量从2开始，每次翻倍
    current_step_size = 2;
    while(current_step_size<=comm_sz){
        //前半部分
        if(my_rank%current_step_size<current_step_size/2)
            dst = my_rank + current_step_size/2;
        else
            dst = my_rank - current_step_size/2;

        //双向交换数据
        MPI_Sendrecv(&local_sum,1,MPI_INT,dst,0,&recv_sum,1,MPI_INT,dst,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        local_sum += recv_sum;

        current_step_size *= 2;
    }

    //将各进程求和结果发送到0进程，打印验证结果
    if(my_rank!=0){
        MPI_Send(&local_sum,1,MPI_INT,0,2,MPI_COMM_WORLD);
    }else{
        printf("my_rank0 locak_sum:%d\n",local_sum);
        for(i=1;i<comm_sz;i++){
            MPI_Recv(&local_sum,1,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("my_rank%d local_sum:%d\n",i,local_sum);
        }
    }

    return 0;
}