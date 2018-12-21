//
// Created by zjp on 18-12-21.
//
#include <iostream>
#include <mpi.h>
#include "../Tools/Tools.h"
#include "sort.h"

/**
 * 串行算法   0.00103641
 * 进程数       时间
 *   1       0.00103998
 *   2       0.00110459
 *   3       0.00132608
 *   8       0.0624137
 *  并行快速排序并没有取得比串行算法更好的性能？？？
 */

using namespace std;
/**
 * 并行快速排序，使用2的m次幂个处理器进行排序
 * @param data
 * @param left
 * @param right
 * @param m 处理机构成的二叉处理树的最大高度为m+1
 * @param id 负责对左半部分进行排序的进程序号
 * @param my_id 当前的进程序号
 */

void para_quickSort(int *data,int left,int right,int m,int id,int my_id);

int main(){
    int n = 10000,max = 100000,m;
    int *data,*verifyArray;
    int my_rank,comm_sz;
    double start_time,end_time;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    if(my_rank==0){
        data = createRandomArray(n,max);
        verifyArray = new int[n];
        copyArray(verifyArray,data,n);
        start_time = MPI_Wtime();
    }
    m = log2(comm_sz);
    para_quickSort(data,0,n-1,m,0,my_rank);

    if(my_rank==0){
        end_time = MPI_Wtime();
        cout << "Time: " << end_time-start_time << endl;
        cout << "实际使用的处理机数：" << exp2(m) << endl;
        qsort(verifyArray,n, sizeof(int),cmp);
        //printArray("After sort",data,n);
        cout << isArrayEqual(data,verifyArray,n) << endl; //验证排序结果
    }

    MPI_Finalize();
    return 0;
}


void para_quickSort(int *data,int left,int right,int m,int id,int my_id){
    int j;
    int pivotpos;
    int rightLength = -1;
    int *temp;

    //如果可供选择的处理器只有一个，那么由处理器id调用串行排序
    if(m==0){
        if(my_id==id) {
            //cout << my_id << endl;
            quickSort(data, left, right);
        }
        return;
    }
    //由id处理器划分数据，并将后一部分数据发送到处理器id+exp2(m-1)
    if(my_id==id){
        pivotpos = partion(data,left,right);
        rightLength = right - pivotpos;

        MPI_Send(&rightLength,1,MPI_INT,id+exp2(m-1),my_id,MPI_COMM_WORLD);
        //如果缓冲区不空，则第id+exp2(m-1)处理器获取数据的首地址是data[r+1]
        if(rightLength!=0)
            MPI_Send(data+pivotpos+1,rightLength,MPI_INT,id+exp2(m-1),my_id,MPI_COMM_WORLD);
    }
    //处理器id+exp2(m-1)接收处理器id发送的消息
    if(my_id==id+exp2(m-1)){
        MPI_Recv(&rightLength,1,MPI_INT,id,id,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        if(rightLength!=0){
            temp = new int[rightLength];
            MPI_Recv(temp,rightLength,MPI_INT,id,id,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
    }

    //递归调用并行排序
    //用2^m-1个处理器对left~(pivotpos-1)的数据进行递归排序
    j = pivotpos-1-left;
    MPI_Bcast(&j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(j>0)
        para_quickSort(data,left,pivotpos-1,m-1,id,my_id);

    //用2^m-1个处理器对r+1~end的数据进行递归排序
    j = rightLength;
    MPI_Bcast(&j,1,MPI_INT,id,MPI_COMM_WORLD);
    if(j>0)
        para_quickSort(temp,0,rightLength-1,m-1,id+exp2(m-1),my_id);

    //处理器id+exp2(m-1)将排序好的数据发回给id处理器
    if(my_id==id+exp2(m-1)&&rightLength!=0)
        MPI_Send(temp,rightLength,MPI_INT,id,id+exp2(m-1),MPI_COMM_WORLD);

    if(my_id==id&&rightLength!=0)
        MPI_Recv(data+pivotpos+1,rightLength,MPI_INT,id+exp2(m-1),id+exp2(m-1),MPI_COMM_WORLD,MPI_STATUS_IGNORE);

}