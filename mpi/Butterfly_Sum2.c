//
// Created by zjp on 18-12-9.
//
/**
 * 蝶形结构通信
 * 适合任意进程数量
 * 在任意进程数的蝶形全局求和中，容易遇到最后一个分组的节点数量不能填满分组容量的状况。
 * 对于节点不足的分组，先将能配对的节点先进行配对通信，然后由这个小组的头节点把小组的局部和发给无法配对的节点。
 *
 * 存在问题：
 * 结果打印异常，只打印部分线程结果？？？
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//求大于或等于commsz的最小2的指数幂
int min_2_exp(int comm_sz);

int main(){
    int my_rank,comm_sz;
    int n =  1004,extend_n =0,local_n,i,no_partner_count,middle_rank;
    int *data,*local_data;
    int src,dst;
    int sum=0,local_sum=0,recv_sum=0;
    int current_step_size,min_2exp;

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
    //求大于或等于comm_sz的最小指数幂
    min_2exp = min_2_exp(comm_sz);
    while(current_step_size<=min_2exp){
        //前半部分
        if(my_rank%current_step_size<current_step_size/2)
            dst = my_rank + current_step_size/2;
        else
            dst = my_rank - current_step_size/2;
        //printf("%d\n",dst);
        //目标进程存在则交换数据
        if(dst<comm_sz) {
            //双向交换数据
            MPI_Sendrecv(&local_sum, 1, MPI_INT, dst, 0, &recv_sum, 1, MPI_INT, dst, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            //printf("MPI_Sendrecv %d %d\n",my_rank,dst);
            local_sum += recv_sum;
        }else{//不存在，由组的头节点把小组的局部和发给无法配对的节点
            dst = my_rank - my_rank%current_step_size;
            if(my_rank!=dst)
                MPI_Recv(&local_sum,1,MPI_INT,dst,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        //每个进程都要检查自己是否需要作为更正器，向无法匹配的进程发送局部和
        if(my_rank%current_step_size==0&&my_rank+current_step_size-1>=comm_sz){
            no_partner_count = my_rank + current_step_size - comm_sz;
            middle_rank = my_rank + current_step_size/2;  //前半部分才会失配
            //printf("current_step_size=%d my_rank=%d no_partner_count=%d middle_rank=%d\n",current_step_size,my_rank,no_partner_count,middle_rank);
            for(;no_partner_count>0;no_partner_count--) {
                dst = middle_rank - no_partner_count;
                if(dst>my_rank&&dst<comm_sz) {
                    MPI_Send(&local_sum, 1, MPI_INT, dst, 2, MPI_COMM_WORLD);
                    //printf("send from %d to %d\n", my_rank, dst);
                }
            }
        }
        current_step_size *= 2;
    }
    //printf("---\n");
    //将各进程求和结果发送到0进程，打印验证结果
    if(my_rank!=0){
        MPI_Send(&local_sum,1,MPI_INT,0,3,MPI_COMM_WORLD);
        //printf("%d send %d\n",my_rank,local_sum);
    }else{
        printf("my_rank0 locak_sum:%d\n",local_sum);
        for(i=1;i<comm_sz;i++){
            MPI_Recv(&local_sum,1,MPI_INT,i,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("my_rank%d local_sum:%d\n",i,local_sum);
        }
    }

    return 0;
}

int min_2_exp(int comm_sz){
    int result = 2;
    if(comm_sz<=2)
        return comm_sz;
    while(result<comm_sz)
        result *= 2;
    return result;
}