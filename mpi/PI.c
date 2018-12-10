//
// Created by zjp on 18-12-10.
//
/**
 * 蒙特卡洛方法求PI
 * 飞镖板边长 2，靶心正中央，中间一个半径为1的圆，面积为PI
 * 假设飞镖落点随机，而且都能投进正方形区域
 * 击中圆内的投掷次数。全部投掷次数 = PI/4
 * 总投掷次数要尽可能大，用long long int
 */

/*
number_in_circle = 0;
for(toss=0;toss<num_of_tosses;toss++){
    x = random double [-1,1]
    y = random double [-1,1]
    distance_square = x*x+y*y
    if(distance_square<=1)
        num_in_circle++
 }
 pi_estimate = 4*number_in_circle/(double)number_of_tosses
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(){
    int my_rank,comm_sz;
    long long int num_of_tosses=0,num_in_circle=0,local_num_in_circle=0;
    int i;
    double x,y,distance_square;
    double pi_estimate;
    double start_time,end_time;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    if(my_rank==0){
        num_of_tosses = 500000000;
        start_time = MPI_Wtime();
    }
    MPI_Bcast(&num_of_tosses,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);

    for(i=0;i<num_of_tosses/comm_sz;i++){
        x = rand()/(double)(RAND_MAX);
        y = rand()/(double)(RAND_MAX);
        distance_square = x*x + y*y;
        if(distance_square<=1)
            local_num_in_circle++;
    }

    MPI_Reduce(&local_num_in_circle,&num_in_circle,1,MPI_LONG_LONG,MPI_SUM,0,MPI_COMM_WORLD);

    if(my_rank==0) {
        pi_estimate = 4 * num_in_circle / (double) num_of_tosses;
        end_time = MPI_Wtime();
        printf("pi estimate : %lf\n",pi_estimate);
        printf("time : %lf\n",end_time-start_time);
    }

    MPI_Finalize();

    return 0;
}