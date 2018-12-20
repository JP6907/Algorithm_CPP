//
// Created by zjp on 18-12-20.
//
/**
 * 秩排序并行化算法
 * MPI
 * mpicxx Sort/RankSort_MPI.cpp Tools/Tools.cpp -o RankSort
 */
/**
 * 10000个数排序
 * 进程数   时间
 *  串行  0.576405
 *   1   0.558153
 *   2   0.282846
 *   3   0.19277
 *   4   0.147336
 *   5   0.236314
 *   6   0.203379
 */
#include "../Tools/Tools.h"
#include <iostream>
#include <mpi.h>

using namespace std;

int main(){
    int my_rank,comm_sz;
    int *array,*position;
    int *verifyArray;
    int n = 10000,local_n;
    double start_time,end_time;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    local_n = n/comm_sz;
    position = new int[n];
    for(int i=0;i<n;i++)
        position[i] = -1;

    if(my_rank==0) {
        array = createRandomArray(n,100);
        //printArray("Before sort",array,n);
        verifyArray = new int[n];
        copyArray(verifyArray,array,n);
        qsort(verifyArray,n, sizeof(int),cmp);

        start_time = MPI_Wtime();

    }else{
        array = new int[n];
    }
    MPI_Bcast(array,n,MPI_INT,0,MPI_COMM_WORLD); //广播待排序数组


    int start = my_rank*local_n;
    int end = start + local_n;
    for(int i=start;i<end;i++){
        int x = 0;
        for(int j=0;j<n;j++){
            if(array[j]<array[i] || (array[i]==array[j]&&j<i))
                x++;
        }
        position[x] = array[i];
    }
    //n可能不能被comm_sz整除以，由主进程负责剩下的
    if(my_rank==0&&n%comm_sz!=0){
        int start = comm_sz*local_n;
        int end = n;
        for(int i=start;i<end;i++){
            int x = 0;
            for(int j=0;j<n;j++){
                if(array[j]<array[i] || (array[i]==array[j]&&j<i))
                    x++;
            }
            position[x] = array[i];
        }
    }
    if(my_rank!=0){
        MPI_Send(position,n,MPI_INT,0,0,MPI_COMM_WORLD);
    }else{
        int *recvPosition = new int[n];
        if(comm_sz>1) {
            for (int i = 1;i<comm_sz;i++){
                MPI_Recv(recvPosition,n,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                for(int k=0;k<n;k++)
                    if(recvPosition[k]!=-1)
                        position[k] = recvPosition[k];
            }
        }
        for(int i=0;i<n;i++)
            array[i] = position[i];

        end_time = MPI_Wtime();
        cout << "Time: " << end_time-start_time << endl;
        //printArray("After sort",array,n);
        cout << isArrayEqual(array,verifyArray,n) << endl;
    }

    MPI_Finalize();
    return 0;
}