//
// Created by zjp on 18-12-11.
//
/**
 * 估算PI
 * PI=4*(1-1/3+1/5-1/7+1/9.....)
 * 利用多线程来计算sum
 * sum 为临界区，需做保护
 * 1. 使用忙等待： while(flag!=my_rank);       flag = (flag+1)%thread_count;
 */

/**
 *串行代码
 * double factor = 1.0
 * double sum = 0.0
 * for(i=0;i<n;i++,factor=-factor){
 *  sum += factor/(2*i+1)
 * }
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long thread_count;
long long n = 1000000;
double sum;
int flag = 0;
//互斥锁
pthread_mutex_t sum_mutex;

void* Thread_sum(void* rank);
int main(){
    long thread;
    pthread_t* thread_handles;

    pthread_mutex_init(&sum_mutex,NULL);

    thread_count = 4;
    thread_handles = malloc(sizeof(pthread_t)*thread_count);

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Thread_sum,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    pthread_mutex_destroy(&sum_mutex);
    free(thread_handles);

    printf("estimate Pi:%lf\n",4*sum);

    return 0;
}


//忙等待
//互斥锁
void* Thread_sum(void* rank){
    long my_rank = (long)rank;
    double factor,my_sum=0.0;
    long long i;
    long long my_n = n/thread_count;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i+my_n;

    if(my_first_i%2==0)
        factor = 1.0;
    else
        factor = -1.0;

    for(i=my_first_i;i<my_last_i;i++,factor=-factor)
        my_sum += factor/(double)(2*i+1);

    printf("%ld %lf\n",my_rank,my_sum);
//    //忙等待
//    while(flag!=my_rank);
//
//    sum+=my_sum;
//    flag = (flag+1)%thread_count;

    //互斥锁
    pthread_mutex_lock(&sum_mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}