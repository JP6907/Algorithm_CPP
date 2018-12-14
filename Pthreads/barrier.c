//
// Created by zjp on 18-12-11.
//
/**
 * 路障的实现
 * 实现所有线程在程序中能达到同一个位置来同步线程
 * 使用信号量实现路障
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

long thread_count = 10;
//信号量路障实现所需要的变量
int counter = 0;
sem_t count_sem; //initialize to 1
sem_t barrier_sem; //initialize to 0 ,初始化为0，不放行所有线程

//条件变量路障实现
int counter2 = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* Thread_work1(void* rank);
void* Thread_work2(void* rank);

int main(){
    long thread;
    pthread_t* thread_handles;

    //信号量实现路障
    sem_init(&count_sem,0,1);
    sem_init(&barrier_sem,0,0);

    thread_handles = malloc(sizeof(pthread_t)*thread_count);

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Thread_work1,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);


    //条件变量实现路障
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_var,NULL);

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Thread_work2,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);


    free(thread_handles);

    return 0;
}

//信号量实现路障
void* Thread_work1(void* rank){
    long my_rank = (long)rank;
    int i;

    printf("rank%ld reach the barrier1 !\n",my_rank);
    sem_wait(&count_sem);
    if(counter==thread_count-1){ //最后一个线程到达这里，不用count++，可以放行所有线程
        counter = 0;
        sem_post(&count_sem);
        for(i=0;i<thread_count;i++)
            sem_post(&barrier_sem);
    }else{
        counter++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
    printf("rank%ld pass the barrier1 !\n",my_rank);

    return NULL;
}

//条件变量实现路障
void* Thread_work2(void* rank){
    long my_rank = (long)rank;

    printf("rank%ld reach the barrier2 !\n",my_rank);

    pthread_mutex_lock(&mutex);
    counter2++;
    if(counter2==thread_count){
        counter2 = 0;
        pthread_cond_broadcast(&cond_var);
    }else{
        while(pthread_cond_wait(&cond_var,&mutex)!=0);
    }
    pthread_mutex_unlock(&mutex);

    printf("rank%ld pass the barrier2 !\n",my_rank);

    return NULL;
}