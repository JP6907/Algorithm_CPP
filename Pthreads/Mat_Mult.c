//
// Created by zjp on 18-12-10.
//
/**
 * 多线程矩阵相乘
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int line_num;  //每个进程计算多少行
float *P,*M,*N;
const int mRow = 1000,mCol = 1000;
const int nRow = 1000,nCol = 1000;

void Mat_Gen(float* mat,int row,int col);
void Mat_Gen_M(float* M,int row,int col);
void* matMult(void* rank);

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;

    //thread_count = strtol(argv[1],NULL,10);
    thread_count = 4;
    line_num = mRow/thread_count;
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    M = malloc(sizeof(float)*mRow*mCol);
    N = malloc(sizeof(float)*nRow*nCol);
    P = malloc(sizeof(float)*mRow*nCol);

    Mat_Gen_M(M,mRow,mCol);
    Mat_Gen(N,nRow,nCol);

    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,matMult,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);

    //打印结果
    //最多打印5行5列
    int i,j;
    int showRow = mRow>5?5:mRow;
    int showCol = nCol>5?5:nCol;
    for(i=0;i<showRow;i++){
        for(j=0;j<showCol;j++)
            printf("%f  ",P[i*mCol+j]);
        printf("...\n");
    }
    printf("\n");
    //最后一行
    for(j=0;j<showCol;j++)
        printf("%f  ",P[(mRow-1)*mCol+j]);
    printf("...\n");

    free(thread_handles);

    return 0;
}

void* matMult(void* rank){
    int i,j,k;
    long my_rank = (long)rank;
    for(i=0;i<line_num;i++)
        for(j=0;j<nCol;j++) {
            float temp = 0.0;
            for (k = 0; k < mCol; k++)
                temp += M[(my_rank*line_num+i)*mCol+k]*N[k*nCol+j];
            P[(my_rank*line_num+i)*nCol+j] = temp;
        }
    return NULL;
}

void Mat_Gen_M(float* M,int row,int col){
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++)
            M[i*row+j] = j;
    }
}

void Mat_Gen(float* mat,int row,int col){
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++)
            mat[i*row+j] = 1.0;
    }
}