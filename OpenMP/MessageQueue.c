//
// Created by zjp on 18-12-13.
//
#include "MessageQueue.h"
#include <stdlib.h>
#include <string.h>

MessageQueue* Create_MessageQueue(int rank){
    MessageQueue* mq = (MessageQueue*)malloc(sizeof(MessageQueue));
    mq->head = NULL;
    mq->tail = NULL;
    mq->enqueueCount = 0;
    mq->dequeueCount = 0;
    mq->my_rank = rank;
    omp_init_lock(&mq->lock);
    return mq;
}

int GetQueueSize(MessageQueue* messageQueue){
    return messageQueue->enqueueCount - messageQueue->dequeueCount;
}

void EnQueue(MessageQueue* messageQueue,char* message_content){
    //入队在tail操作
    if(GetQueueSize(messageQueue)==0){
        messageQueue->head = malloc(sizeof(Message));
        messageQueue->head->message = message_content;
        messageQueue->tail = messageQueue->head;
    }else{
        Message* newMessage = malloc(sizeof(Message));
        newMessage->message = message_content;
        newMessage->next = NULL;
        messageQueue->tail->next = newMessage;
        messageQueue->tail = newMessage;
    }
    messageQueue->enqueueCount++;
};

void DeQueue(MessageQueue* messageQueue,char* message_content){
    //出队在head操作
    if(GetQueueSize(messageQueue)!=0){
        Message* head = messageQueue->head;
        strncpy(message_content,head->message, sizeof(message_content));
        messageQueue->head = messageQueue->head->next;
        free(head);
        messageQueue->dequeueCount++;
    }
}