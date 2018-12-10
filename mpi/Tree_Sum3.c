//
// Created by zjp on 18-12-10.
//
/**
 * 树形结构通行
 * 效果跟MPI_Reduce相同
 *
 * 存在问题：
 * 进程数量超过一定数量之后，没有打印结果
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main() {
    int my_rank, comm_sz, current_comm_sz;
    int sum = 0, local_sum = 0, recv_sum = 0, local_n, dst, src;
    int n = 1000, extend_n = 0, i, temp;
    int *data, *local_data;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //如果n不能被进程数整除以，补充长度到可以被整除
    if (n % comm_sz != 0)
        extend_n = comm_sz * (n / comm_sz + 1) - n;
    //printf("%d\n",n);

    local_n = (n + extend_n) / comm_sz;
    data = (int *) malloc(sizeof(int) * (n + extend_n));
    local_data = (int *) malloc(sizeof(int) * local_n);

    current_comm_sz = comm_sz;

    //init data
    if (my_rank == 0) {
        for (i = 0; i < n; i++)
            data[i] = i;
    }
    //分发数据
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    //计算local sum
    for (i = 0; i < local_n; i++)
        local_sum += local_data[i];
    //printf("%d  local_sum : %d\n", my_rank, local_sum);

    MPI_Reduce(&local_sum,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(my_rank==0){
        printf("my_rank%d The sum is: %d\n",my_rank,sum);
    }
}