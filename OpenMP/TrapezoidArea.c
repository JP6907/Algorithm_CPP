//
// Created by zjp on 18-12-12.
//
/**
 * 使用openmp多线程计算梯形积分
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);
void Trap(double a,double b,int n,double* global_result_p);

int main(){
    double global_result = 0.0;
    double a = 0,b = 3;
    int n = 100000000;
    int thread_count = 12;

#pragma omp parallel num_threads(thread_count)
    Trap(a,b,n,&global_result);

    printf("The result:%lf\n",global_result);

    return 0;
}


void Trap(double a,double b,int n,double* global_result_p){
    double h,x,my_result;
    double local_a,local_b;
    int i,local_n;
    int my_rank =  omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    h = (b-a)/n;
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    my_result = (f(local_a)+f(local_b))/2.0;
    for(i=1;i<=local_n-1;i++){
        x = local_a + i*h;
        my_result += f(x);
    }
    //无法被线程数整除，由最后一个线程计算剩下的部分
    if(n%thread_count!=0){
        if(my_rank==thread_count-1){
            my_result += (f(local_a+h)+f(b))/2.0;
            for(x=local_b+2*h;x<b;x+=h)
                my_result += f(x);
        }
    }
    my_result *= h;

//实现互斥写全局变量
#pragma omp critical
    *global_result_p += my_result;
}

double f(double x){
    return x*x;
}