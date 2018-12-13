//
// Created by zjp on 18-12-13.
//
/**
 * 三种方式实现互斥访问
 * critical 互斥的是整个代码块
 * atomic 只能保护一条C语言赋值语句形成的临界区
 * lock 适用于需要互斥的是某个数据结构而不是代码块的情况
 */
#include "MessageQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int thread_count = 5;
MessageQueue msg_queue[5];
int done_sending = 0;   //记录已完成发送消息的进程数
int send_max = 3; //每个线程发送消息的次数

char* genRandomMsg(int my_rank,int length);
void Send_Message(int dest,char* message_content);
void Try_Receive_Message(int my_rank);
int Is_Done(int my_rank);
void Send_and_Recv();

int main(){

#pragma omp parallel num_threads(thread_count)
    Send_and_Recv();

    int i;
    for(i=0;i<thread_count;i++){
        omp_destroy_lock(&msg_queue[i].lock);
        free(msg_queue[i].head);
        free(msg_queue[i].tail);
        //free(&msg_queue[i]);
    }
    return 0;
}

void Send_and_Recv(){
    int my_rank = omp_get_thread_num();
    //初始化该线程的消息队列
    msg_queue[my_rank] = *Create_MessageQueue(my_rank);
    //必须所有线程完成初始化消息队列后才能开始发送消息
#pragma omp barrier
    printf("rank%d init over\n",my_rank);

    char* message_content = genRandomMsg(my_rank,10);
    int i;
    for(i=0;i<send_max;i++){
        Send_Message((my_rank+i+1)%thread_count,message_content);
        Try_Receive_Message(my_rank);
    }
#pragma omp atomic
    done_sending++;
    while(Is_Done(my_rank)==0)  //还有消息未被接收
        Try_Receive_Message(my_rank);
}

void Send_Message(int dest,char* message_content){
    //入队需要在目的链表尾部操作，需互斥访问
    ///使用critical方式
//#pragma omp critical
//    EnQueue(&msg_queue[dest],message_content);
    ///使用锁方式
    omp_set_lock(&msg_queue[dest].lock);
    EnQueue(&msg_queue[dest],message_content);
    omp_unset_lock(&msg_queue[dest].lock);
}

void Try_Receive_Message(int my_rank){
    int queue_size = GetQueueSize(&msg_queue[my_rank]);
    if(queue_size==0) {
        return;
    }
    else {
        char* msg = malloc(sizeof(char)*MAX_MESSAGE);
        if (queue_size == 1) {  //不能同时入队出队
        ///使用critical方式
//#pragma omp critical
//            DeQueue(&msg_queue[my_rank], msg);
        ///使用锁方式
        omp_set_lock(&msg_queue[my_rank].lock);
        DeQueue(&msg_queue[my_rank], msg);
        omp_unset_lock(&msg_queue[my_rank].lock);
        } else { //可以同时入队出队
            DeQueue(&msg_queue[my_rank], msg);
        }
        printf("rank%d of %d recv msg : %s\n", my_rank,omp_get_num_threads(),msg);
    }
}
int Is_Done(int my_rank){
    if(GetQueueSize(&msg_queue[my_rank])==0 && done_sending==thread_count)
        return 1;
    else
        return 0;
}



char* genRandomMsg(int my_rank,int length) {
    int flag, i;
    char *string;
    if ((string = (char *)malloc(sizeof(char)*length)) == NULL) {
        return NULL;
    }
    string[0] = '0' + my_rank;
    for (i = 1; i < length; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                string[i] = 'A' + rand() % 26;
                break;
            case 1:
                string[i] = 'a' + rand() % 26;
                break;
            case 2:
                string[i] = '0' + rand() % 10;
                break;
            default:
                string[i] = 'x';
                break;
        }
    }
    string[length - 1] = '\0';
    return string;
}