//
// Created by zjp on 18-12-11.
//
/**
 * 使用信号量让线程发送消息
 * 初始状态，所有信号量初始化为0，全部加锁
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MSG_MAX 100

//全局消息，实现消息传递
char** messages;
long thread_count = 10;
//信号量
sem_t* semaphores;

void* Send_msg(void* rank);

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    ///这一句编译时会出现段错误？？？？？？？？？？？？？？？？？？？？？？？？？
    //thread_count = strtol(argv[1],NULL,10);

    thread_handles = malloc(sizeof(pthread_t)*thread_count);
    semaphores = malloc(sizeof(sem_t)*thread_count);
    messages = malloc(sizeof(char)*thread_count*MSG_MAX);

    for(thread=0;thread<thread_count;thread++) {
        //初始化信号量 ，第二个参数不是用，传0
        sem_init(&semaphores[thread],0,0);
        pthread_create(&thread_handles[thread], NULL, Send_msg, (void *) thread);
    }

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread], NULL);

    for(thread=0;thread<thread_count;thread++)
        sem_destroy(&semaphores[thread]);


    free(thread_handles);

    return 0;
}

void* Send_msg(void* rank){
    long my_rank = (long)rank;
    long dest = (my_rank+1)%thread_count;
    char* my_msg = malloc(sizeof(char)*MSG_MAX);

    sprintf(my_msg,"Hello to %ld from %ld",dest,my_rank);
    messages[dest] = my_msg;
    //unlock dst
    sem_post(&semaphores[dest]);

    sem_wait(&semaphores[my_rank]);
    printf("Thread %ld get messages : %s \n",my_rank,messages[my_rank]);

    return NULL;
}