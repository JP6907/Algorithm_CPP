//
// Created by zjp on 18-12-10.
//
/**
 * 蝶形结构通信
 * 效果和MPI_AllReduce一样
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(){
    int my_rank,comm_sz;
    int n =  1004,extend_n =0,local_n,i;
    int *data,*local_data;
    int sum=0,local_sum=0;

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

    MPI_Allreduce(&local_sum,&sum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    //将各进程求和结果发送到0进程，打印验证结果
    if(my_rank!=0){
        MPI_Send(&sum,1,MPI_INT,0,3,MPI_COMM_WORLD);
        //printf("%d send %d\n",my_rank,local_sum);
    }else{
        printf("my_rank0 sum:%d\n",sum);
        for(i=1;i<comm_sz;i++){
            MPI_Recv(&sum,1,MPI_INT,i,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("my_rank%d sum:%d\n",i,sum);
        }
    }

    return 0;
}
