//
// Created by zjp on 18-12-9.
//
/**
 * 树形结构通信
 * 通信域comm_sz的进程数是2的幂的特殊情况
 * https://blog.csdn.net/zhangjt33/article/details/81163308
 * 存在问题：分发总和结果时，只有1进程能收到0进程分发的数据，也就是只进行一次分发
 *
 * 注：数据数量必须能被进程数量整除以,如果不能，需要补充长度到可以被整除
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(){
    int my_rank,comm_sz,current_comm_sz;
    int sum=0,local_sum=0,recv_sum=0,local_n,dst,src;
    int n = 10000,i;
    int *data,*local_data;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    local_n = n/comm_sz;
    data = (int*)malloc(sizeof(int)*n);
    local_data = (int*)malloc(sizeof(int)*local_n);

    current_comm_sz = comm_sz;

    //init data
    if(my_rank==0){
        for(i=0;i<n;i++)
            data[i] = i;
    }
    //分发数据
    MPI_Scatter(data,local_n,MPI_INT,local_data,local_n,MPI_INT,0,MPI_COMM_WORLD);
    //计算local sum
    for(i=0;i<local_n;i++)
        local_sum += local_data[i];
    printf("%d  local_sum : %d\n",my_rank,local_sum);
    //计算总和
    //当前活跃进程不少于1，而且当前进程在活跃进程内
    while(current_comm_sz>1&&my_rank<current_comm_sz){
        if(my_rank>=current_comm_sz/2){
            dst = my_rank - current_comm_sz/2;
            MPI_Send(&local_sum,1,MPI_INT,dst,0,MPI_COMM_WORLD);
        }else{
            src = my_rank + current_comm_sz/2;
            MPI_Recv(&recv_sum,1,MPI_INT,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            local_sum += recv_sum;
        }
        current_comm_sz /= 2;
    }
    //计算总和分发到各个进程
    if(my_rank==0){
        sum = local_sum;
        printf("my_rank%d The sum is: %d\n",my_rank,sum);
    }
    //活跃进程数量为2开始分发
    current_comm_sz = 2;
    //本进程处在分发范围内
    while(current_comm_sz<=comm_sz&&my_rank<current_comm_sz){
        //printf("%d  %d  %d\n",++i,current_comm_sz,my_rank);
        //前半部分是发送进程，后半部分是接收进程
        if(my_rank>=current_comm_sz/2){
            src = my_rank - current_comm_sz/2;
            MPI_Recv(&sum,1,MPI_INT,src,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("my_rank%d Recv from %d ,The sum is: %d\n",my_rank,src,sum);
        }else{
            dst = my_rank + current_comm_sz/2;
            MPI_Send(&sum,1,MPI_INT,dst,1,MPI_COMM_WORLD);
        }
        current_comm_sz *= 2;
    }
    //将各进程收到的sum发送到0进程打印，验证分发结果
    if(my_rank!=0)
        MPI_Send(&sum,1,MPI_INT,0,2,MPI_COMM_WORLD);
    else{
        for(i=1;i<comm_sz;i++) {
            MPI_Recv(&sum,1,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("my_rank%d The sum is: %d\n", i, sum);
        }
    }

    MPI_Finalize();
    return 0;
}
