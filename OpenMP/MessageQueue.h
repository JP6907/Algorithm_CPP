//
// Created by zjp on 18-12-13.
//

#ifndef PARALLELPROGRAM_MESSAGEQUEUE_H
#define PARALLELPROGRAM_MESSAGEQUEUE_H

#include <omp.h>

#define MAX_MESSAGE 50

typedef struct Message{
    char* message;
    struct Message* next;
}Message;

typedef struct MessageQueue{
    int my_rank;  //线程标志
    struct Message* head;  //队首指针
    struct Message* tail;  //队尾指针
    int enqueueCount;  //入队消息数目
    int dequeueCount;  //出队消息数目
    omp_lock_t lock;  //锁
}MessageQueue;

MessageQueue* Create_MessageQueue(int rank);
int GetQueueSize(MessageQueue* messageQueue);
void EnQueue(MessageQueue* messageQueue,char* message_content);
void DeQueue(MessageQueue* messageQueue,char* message_content);

#endif //PARALLELPROGRAM_MESSAGEQUEUE_H
