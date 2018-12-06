//
// Created by zjp on 18-12-5.
//
/**
 * 使用梯形积分法计算 f(x)在区间[a,b]的积分面积
 */

#include <mpi.h>
#include <stdio.h>

double f(double x);
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len);

int main(){

    double starttime,endtime;  //计算时间
    int my_rank,comm_sz,n=1024,local_n;
    double a = 0.0,b = 3.0,h,local_a,local_b;
    double local_int,total_int; //area
    int source;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    starttime = MPI_Wtime();

    h = (b-a)/n;
    local_n = n/comm_sz;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a,local_b,local_n,h);

    //使用send recive汇总数据
//    if(my_rank!=0){
//        MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
//    }else{ //全局求和
//        total_int = local_int;
//        //按顺序接收数据
//        for(source = 1;source<comm_sz;source++){
//            MPI_Recv(&local_int,1,MPI_DOUBLE,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//            total_int += local_int;
//        }
//    }

    //使用reduce归约函数求和
    MPI_Reduce(&local_int,&total_int,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    if(my_rank==0){
        printf("Calcule the Trapezoid Area\n");
        printf("n = %d, [%f,%f], estimate area = %lf\n",n,a,b,total_int);

        endtime = MPI_Wtime();
        printf("Time:%lf \n",endtime-starttime);
    }
    MPI_Finalize();

    return 0;
}





double f(double x){
    return x*x*x;
}
/**
 * @param left_endpt 左横坐标
 * @param right_endpt 右横坐标
 * @param trap_count 分成多少过等长子区间
 * @param base_len 每个区间的长度
 * @return 梯形面积
 */
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len){
    double estimate,x;
    int i;

    estimate = (f(left_endpt)+f(right_endpt))/2.0;
    for(i=1;i<=trap_count-1;i++){
        x = left_endpt + i*base_len;
        estimate += f(x);
    }
    estimate = estimate*base_len;
    return estimate;
}