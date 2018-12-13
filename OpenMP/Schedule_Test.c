//
// Created by zjp on 18-12-13.
//
/**
 * 测试OpenMP的各种调度策略
 * gcc -g -Wall -lm -fopenmp Schedule_Test.c -o Schedule_Test
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

//该函数计算时间随i的增大而增大
double f(int i);

int main(){
    double sum = 0.0;
    int n = 10000,i;
    int thread_count = 10;

    struct timeval start;
    struct timeval end;
    unsigned long diff;

    //无并行
    gettimeofday(&start,NULL);
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("no parallel time:%ld\n",diff);

    //no schedule
    gettimeofday(&start,NULL);
#pragma omp parallel for num_threads(thread_count) \
    reduction(+:sum)
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("no schedule time:%ld\n",diff);

    //static,1
    gettimeofday(&start,NULL);
#pragma omp parallel for num_threads(thread_count) \
    reduction(+:sum) schedule(static,1)
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("static,1 time:%ld\n",diff);

    //dynamic,1
    gettimeofday(&start,NULL);
#pragma omp parallel for num_threads(thread_count) \
    reduction(+:sum) schedule(dynamic,1)
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("dynamic,1 time:%ld\n",diff);

    //dynamic,10
    gettimeofday(&start,NULL);
#pragma omp parallel for num_threads(thread_count) \
    reduction(+:sum) schedule(dynamic,10)
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("dynamic,10 time:%ld\n",diff);


    //guided,1
    gettimeofday(&start,NULL);
#pragma omp parallel for num_threads(thread_count) \
    reduction(+:sum) schedule(guided,5000)
    for(i=0;i<n;i++)
        sum += f(i);
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("guided,5000 time:%ld\n",diff);


    return 0;
}


double f(int i){
    int j,start = i*(i+1)/2,finish=start +i;
    double return_val = 0.0;
    for(j=start;j<finish;j++)
        return_val += sin(j);
    return return_val;
}