//
// Created by zjp on 18-12-10.
//
/**
 * 矩阵相乘运算
 * 使用 scatter 和 gather
 * 行数必须能够被进程数整除，否则必须增加若干零行到能够被整除
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Mat_Gen(float* mat,int row,int col);
void Mat_Gen_M(float* M,int row,int col);


int main(){

    double start_time,end_time;
    int my_rank,comm_sz;
    float *M,*N,*P,*buffer,*ans;  //buffer用于保存分块结果，ans用于保存分块计算结果
    int mRow = 1000,mCol = 1000;  //距阵M行数、列数
    int nRow = mCol,nCol = 1000;  //距阵N行数、列数
    //////// M的列数必须等于N的行数
    //int width = 1000;
    int i,j,k;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    int line_per_block = mRow/comm_sz; //每个进程多少行
    M = (float*)malloc(sizeof(float)*mRow*mCol);
    N = (float*)malloc(sizeof(float)*nRow*nCol);
    P = (float*)malloc(sizeof(float)*mRow*nCol);
    //缓存大小大于等于要处理的数据的大小，大于时只需关注实际数据部分
    buffer = (float*)malloc(sizeof(float)*line_per_block*mCol); //保存分块结果
    ans = (float*)malloc(sizeof(float)*line_per_block*nCol); //保存数据块计算结果

    //在0进程生成数据
    if(my_rank==0){
        Mat_Gen_M(M,mRow,mCol);
        Mat_Gen(N,nRow,nCol);
        start_time = MPI_Wtime();
    }
    //广播矩阵N
    MPI_Bcast(N,nRow*nCol,MPI_FLOAT,0,MPI_COMM_WORLD);
    //分发M的各块
    MPI_Scatter(M,line_per_block*mCol,MPI_FLOAT,buffer,line_per_block*mCol,MPI_FLOAT,0,MPI_COMM_WORLD);
    //各个进程计算结果
    for(i=0;i<line_per_block;i++){
        for(j=0;j<nCol;j++){
            float temp = 0.0;
            for(k=0;k<mCol;k++){
                temp += buffer[i*mCol+k]*N[k*nCol+j];
            }
            ans[i*nCol+j] = temp;
        }
    }
    //聚集各进程结果到0进程
    MPI_Gather(ans,line_per_block*nCol,MPI_FLOAT,P,line_per_block*nCol,MPI_FLOAT,0,MPI_COMM_WORLD);

    //打印结果
    if(my_rank==0){
        end_time = MPI_Wtime();
        printf("time : %f\n",end_time-start_time);
        //最多打印5行5列
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
    }

    MPI_Finalize();

    return 0;
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