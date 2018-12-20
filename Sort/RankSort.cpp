//
// Created by zjp on 18-12-20.
//
/**
 * 秩排序的基本思想为：
 * 统计出小于每个被选数的个数，个数值即为被选择的数在序列中的位置
 */
/**
 * 10000个数排序
 * 时间：0.576405
 */

#include "../Tools/Tools.h"
#include <sys/time.h>
#include <mpi.h>
#include <iostream>

using namespace std;

void rankSort(int *a,int len);

int main(){
    MPI_Init(NULL,NULL);
    int n=10000 ,max=50;
    double start;
    double end;
    int *array = createRandomArray(n,max);
    //printArray("Before sort",array,n);

    start = MPI_Wtime();
    rankSort(array,n);
    end = MPI_Wtime();
    cout << "Time: " << end-start << endl;
    //printArray("After Sort",array,n);

    MPI_Finalize();
    return 0;
}

void rankSort(int *a,int len){
    int *b = new int[len];
    for(int i=0;i<len;i++){
        int x=0;
        for(int j=0;j<len;j++){
            //数组中可能存在相同的数，为实现稳定排序，规定在相同的数中，原序号更大的数“更大”
            if(a[j]<a[i] || (a[j]==a[i]&&j<i))
                x++;
        }
        b[x] = a[i];
    }
    for(int i=0;i<len;i++)
        a[i] = b[i];
    delete []b;
//    delete []a;
//    a = b;
}
