//
// Created by zjp on 18-12-10.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct return_type{
    long my_rank;
};

//全局共享内存变量
int thread_count;

void* Hello(void* rank);

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    void* pth_return;

    thread_count = strtol(argv[1],NULL,10); //将字符串转化成整型

    thread_handles = malloc(thread_count* sizeof(pthread_t));

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Hello,(void*)thread);

    printf("Hello from main thread\n");

    for(thread=0;thread<thread_count;thread++) {
        pthread_join(thread_handles[thread], &pth_return);
        printf("return from thread %ld : %ld\n",thread,((struct return_type*)pth_return)->my_rank);
    }

    free(thread_handles);

    return 0;
}

void* Hello(void* rank){
    //64位系统指针类型是64位，int是32位，为了避免警告，用long代替int
    long my_rank = (long)rank;
    printf("Hello from thread %ld of %d\n",my_rank,thread_count);
    struct return_type return_value;
    return_value.my_rank = my_rank;
    pthread_exit((void*)&return_value);
}