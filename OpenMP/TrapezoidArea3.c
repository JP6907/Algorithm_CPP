//
// Created by zjp on 18-12-12.
//
/**
 * 使用openmp多线程计算梯形积分
 * 使用 parallel for 显示并行化梯形积分法
 * parallel for 只能并行化那些迭代次数在循环之前就可以确定的for，唯一例外是，在循环体中可以有一个exit调用
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);

int main(){
    double global_result = 0.0;
    double a = 0,b = 3 ,h;
    int n = 100000000;
    int thread_count = 10;

    h = (b-a)/n;
    global_result = (f(a)+f(b))/2.0;
#pragma omp parallel for num_threads(thread_count) \
    reduction(+: global_result)
    for (int i = 1; i <= n-1; ++i) {
        global_result += f(a+i*h);
    }
    global_result *= h;

    printf("The result:%lf\n",global_result);

    return 0;
}



double f(double x){
    return x*x;
}