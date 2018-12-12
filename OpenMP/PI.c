//
// Created by zjp on 18-12-12.
//
/**
 * 数据依赖性问题
 * 存在两种依赖性：数据依赖和循环依赖，一般需要注意的是循环依赖
 * 当一个共享变量在一次迭代中计算，其结果在下一次迭代或之后的迭代会被使用到，就产生了循环依赖
 *
 * 一个估算PI的串行化方法：
 * double factor = 1.0;
    double sum = 0.0,pi_estimate;
    int n = 1000000;

    for(int i=0;i<n;i++){
        sum += factor/(2*i+1);
        factor = -factor;
    }
    pi_estimate = 4.0*sum;

    可以发现，factor是一个全局共享,在第k次迭代中，factor的更新和下一次迭代的sum累加是一个循环依赖
    多个线程可能同时对factor同时进行操作，产生不确定结果
    解决方法：每个进程通过迭代次数来计算当前的factor值，不依赖与上一次迭代
    使每个线程都有自己的factor副本，互不干扰
    添加一个private私有作用域
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(){
    int thread_count = 10;
    double factor = 1.0;
    double sum = 0.0,pi_estimate;
    int n = 1000000,i;

#pragma omp parallel for num_threads(thread_count) \
    default(none) reduction(+:sum) private(i,factor) shared(n)
    for(i=0;i<n;i++){
        if(i%2==0)
            factor = 1.0;
        else
            factor = -1.0;
        sum += factor/(2*i+1);
    }
    pi_estimate = 4.0*sum;

    printf("estimate pi : %lf\n",pi_estimate);

    ///一个私有作用域变量的值在parallel块或parallel for块的开始是未指定的，在完成之后也是未指定
    int x = 666;
#pragma omp parallel num_threads(thread_count) \
    private(x)
    {
        int my_rank = omp_get_thread_num();
        printf("%d  %d\n",my_rank,x);  //x不是等于666，未初始化
        x = my_rank;
        printf("%d  %d\n",my_rank,x);
    }
    printf("after block %d\n",x);

    return 0;
}