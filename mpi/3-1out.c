//
// Created by zjp on 18-12-5.
//
/**
 * 由一个进程负责所有输出
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(){
    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;
    int source;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if(my_rank!=0){  //其它进程把消息发送到0进程
        sprintf(greeting,"Greetings from process %d of %d \n",my_rank,comm_sz); //输出到字符串中
        MPI_Send(greeting,strlen(greeting)+1,MPI_CHAR,0,0,MPI_COMM_WORLD); //msg_size包含字符串结束符'\0'
    }else{ //0进程负责打印
        printf("Greetings from process %d of %d \n",my_rank,comm_sz);
        //按进程顺序接收
//        for(source=1;source<comm_sz;source++){
//            MPI_Recv(greeting,MAX_STRING,MPI_CHAR,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//            printf("%s\n",greeting);
//        }
        //按进程完成工作顺序接收  MPI_ANY_SOURCE
        for(source=1;source<comm_sz;source++){
            MPI_Recv(greeting,MAX_STRING,MPI_CHAR,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("%s\n",greeting);
        }
    }

    MPI_Finalize();
    return 0;
}