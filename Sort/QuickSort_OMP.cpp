//
// Created by zjp on 18-12-21.
//
#include "Tools.h"
#include "sort.h"
#include "omp.h"
#include <time.h>
#include <iostream>

/**
 * g++ -g -Wall Sort/QuickSort_OMP.cpp Sort/sort.cpp Tools/Tools.cpp -I Tools/ -o QuickSort_OMP
 *
 * 2线程并行算法时间和串行算法时间相近？？？？
 */

using namespace std;

void para_quicksort(int *a,int left,int right,int treeHigh);
void test(int n);

int main(){
    //int treeHigh = 4;
    omp_set_num_threads(2);
    clock_t start_time,end_time;
    int n=1000000,max=1000000;
    int *a = createRandomArray(n,max);
    int *b = new int[n];
    copyArray(b,a,n);
    qsort(b,n,sizeof(int),cmp);
    //para_quicksort(a,0,n-1,treeHigh);
    start_time = clock();
    int pivotpos = partion(a,0,n-1);
#pragma omp parallel sections
    {
#pragma omp section
        {
            quickSort(a, 0, pivotpos - 1);
            cout << "quick " << omp_get_thread_num() << endl;
        }
#pragma omp section
        {
            quickSort(a, pivotpos + 1, n - 1);
            cout << "quick " << omp_get_thread_num() << endl;
        }
    }
    end_time = clock();
    cout << "Time:" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
    cout << isArrayEqual(a,b,n) << endl;
    //printArray("After sort",a,n);


    return 0;
}

void test(int n){
    if(n>0){
        cout << "Hello from " << omp_get_thread_num() << endl;
#pragma omp parallel sections
            {
#pragma omp section
                test(n-1);
#pragma omp section
                test(n-1);
        }
    }
}

void para_quicksort(int *a,int left,int right,int treeHigh){
    if(left<right){
        if(treeHigh==1) {
            quickSort(a, left, right);
            cout << omp_get_thread_num() << " of " << omp_get_num_threads() << endl;
        }
        else {
            int pivot = partion(a, left, right);
#pragma omp parallel sections
            {
#pragma omp section
                para_quicksort(a, left, pivot - 1, treeHigh - 1);
#pragma omp section
                para_quicksort(a, pivot + 1, right, treeHigh - 1);
            }
        }
    }
}