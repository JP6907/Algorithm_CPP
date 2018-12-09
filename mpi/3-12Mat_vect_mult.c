//
// Created by zjp on 18-12-5.
//
/**
 * MPI矩阵-向量乘法计算
 * 使用的进程数必须能被M的行数整除
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N_FLAG 0
#define BUFFER_FLAG 1
#define ANS_FLAG 2

void Mat_Mult(float* M,int mRow,int mCol,float* N,int nRow,int nCol);
void Mat_Gen(float* mat,int row,int col);

void Mat_Gen(float* mat,int row,int col){
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++)
            mat[i*row+j] = 1.0;
    }
}
void Mat_Gen_M(float* M,int row,int col){
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++)
            M[i*row+j] = j;
    }
}


int main(){

    double starttime,endtime;
    int my_rank,comm_sz;
    float *M,*N,*P,*buffer,*ans;  //buffer用于保存分块结果，ans用于保存分块计算结果
    int mRow = 1000,mCol = 1000;  //距阵M行数、列数
    int nRow = mCol,nCol = 1000;  //距阵N行数、列数
    //////// M的列数必须等于N的行数
    //int width = 1000;
    int i,j,k;

    MPI_Init(NULL,NULL);
    starttime = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    int line_per_block = mRow/comm_sz; //每个进程多少行
    M = (float*)malloc(sizeof(float)*mRow*mCol);
    N = (float*)malloc(sizeof(float)*nRow*nCol);
    P = (float*)malloc(sizeof(float)*mRow*nCol);
    //缓存大小大于等于要处理的数据的大小，大于时只需关注实际数据部分
    buffer = (float*)malloc(sizeof(float)*line_per_block*mCol); //保存分块结果
    ans = (float*)malloc(sizeof(float)*line_per_block*nCol); //保存数据块计算结果

    //主进程初始化矩阵，并将矩阵N广播到各进程，矩阵M分组广播到各进程
    if(my_rank==0){
        Mat_Gen_M(M,mRow,mCol);
        Mat_Gen(N,nRow,nCol);
        //广播矩阵N
        /**
         * 为何使用MPI_Bcast会阻塞在这里？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
         * MPI_Bcast可以指定FLAG？？？
         */
        //MPI_Bcast(N,nRow*nCol,MPI_FLOAT,N_FLAG,MPI_COMM_WORLD);
        for(i=1;i<comm_sz;i++){
            MPI_Send(N,nRow*nCol,MPI_FLOAT,i,N_FLAG,MPI_COMM_WORLD);
        }
        //分发M的各块
        for(i=1;i<comm_sz;i++){
            MPI_Send(M+i*line_per_block*mCol,line_per_block*mCol,MPI_FLOAT,i,BUFFER_FLAG,MPI_COMM_WORLD);
        }
        /**
         * 为何使用MPI_Scatter会阻塞在这里？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
         * MPI_Scatter可以指定FLAG？？？
         */
        //MPI_Scatter(M,line_per_block*mCol,MPI_FLOAT,buffer,line_per_block*mCol,MPI_FLOAT,0,MPI_COMM_WORLD);
        //接收从进程计算结果
        for(k=1;k<comm_sz;k++){
            MPI_Recv(ans,line_per_block*nCol,MPI_FLOAT,k,ANS_FLAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            //将结果传递给数组P
            for(i=0;i<line_per_block;i++){
                for(j=0;j<nCol;j++){
                    P[(k*line_per_block+i)*nCol+j] = ans[i*nCol+j];
                }
            }
        }
        //计算第0块数据
        for(i=0;i<line_per_block;i++){
            for(j=0;j<nCol;j++){
                float temp = 0.0;
                for(k=0;k<mCol;k++){
                    temp += M[i*mCol+k]*N[k*nCol+j];
                }
                P[i*nCol+j] = temp;
            }
        }

        endtime = MPI_Wtime();

        //测试结果，计算一行的和
        float sum1 = 0;
        float sum2 = 0;
        for(i=0;i<nCol;i++){
            sum1+=M[i];
            sum2+=P[2*nCol+i];
        }
        printf("sum1=%f,sum2=%f\n",sum1,sum2);
        //最多打印5行5列
        int showRow = mRow>5?5:mRow;
        int showCol = nCol>5?5:nCol;
        for(i=0;i<showRow;i++){
            for(j=0;j<showCol;j++)
                printf("%f  ",P[i*mRow+j]);
            printf("...\n");
        }
        //统计时间

        printf("my_rank:%d,time:%lf\n",my_rank,endtime-starttime);
    }else{
        //接收广播的数据
        MPI_Recv(N,nRow*nCol,MPI_FLOAT,0,N_FLAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(buffer,line_per_block*mCol,MPI_FLOAT,0,BUFFER_FLAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        //计算乘积，并将结果发送给主进程
        for(i=0;i<line_per_block;i++){
            for(j=0;j<nCol;j++){
                float temp = 0.0;
                for(k=0;k<mCol;k++){
                    temp += buffer[i*mCol+k]*N[k*nCol+j];
                }
                ans[i*nCol+j] = temp;
            }
        }
        //将计算结果发送给0进程
        MPI_Send(ans,line_per_block*nCol,MPI_FLOAT,0,ANS_FLAG,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

/**
 * 测试环境 cpu：i7  4核
 * 进程数   消耗时间
 *   1     4.933558
 *   2     5.162811
 *   3     3.263601
 *   4     2.593954
 *   5     3.018138
 *   6     2.954932
 *   7     2.536357
 *   8     2.824797
 *
 *   20    3.997551
 *
 * 消耗时间不是一直随着进程数的增加而保持减少的趋势
 * cpu一个核只能运行一个进程，在进程数超过cpu实际的物理核数之后
 * 进程的推进需要一直保持换入换出的状态，进程的置换需要耗费一定的时间
 */