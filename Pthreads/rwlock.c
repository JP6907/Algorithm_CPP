//
// Created by zjp on 18-12-14.
//
/**
 * 读写锁
 * 使用读写锁来限制同一数据多线程读写。若任何线程拥有读锁，则其他任何请求写锁的线程将阻塞在其写锁函数的调用上；
 * 若任何线程拥有写锁，则其他任何请求读锁和写锁的线程将阻塞在其对应的锁函数上，相当于将读与写的互斥量分开，且写入锁的要求更高一些。
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DATA_SIZE 32768
int thread_count = 10;
int data[DATA_SIZE];
int workCount = 10000;

pthread_rwlock_t rwlock;

void* work(void* rank);

int main(){
    int i;
    long thread;
    pthread_t * thread_handles;

    thread_handles = malloc(sizeof(pthread_t)*thread_count);
    pthread_rwlock_init(&rwlock,NULL);

    for(i=0;i<DATA_SIZE;i++)
        data[i] = rand();

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,work,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    pthread_rwlock_destroy(&rwlock);
    free(thread_handles);

    printf("Finish!\n");

    return 0;
}


void* work(void* rank){
    long my_rank = (long)rank;
    int i,temp;
    for(i=0;i<workCount;i++){
        if(rand()%2){
            pthread_rwlock_rdlock(&rwlock);  //读过程，上锁
            temp = data[i];
            pthread_rwlock_unlock(&rwlock);
        }else{
            pthread_rwlock_wrlock(&rwlock); //写过程
            data[i] = rand();
            pthread_rwlock_unlock(&rwlock);
        }
    }
    return NULL;
}