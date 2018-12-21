//
// Created by zjp on 18-12-21.
//
#include "catch.h"
#include "sort.h"
#include "Tools.h"
#include <mpi.h>

#define RANK_SORTED
#define QUICK_SORT

using namespace std;

#ifdef RANK_SORT
TEST_CASE("rankSort"){
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
}
#endif


#ifdef QUICK_SORT
///10000个数排序 ，时间 0.00103641
TEST_CASE("quickSort"){
    MPI_Init(NULL,NULL);
    int n = 10000;
    int max = 100000;
    double start;
    double end;

    int *a = createRandomArray(n, max);
    int *b = new int[n];
    copyArray(b, a, n);
    //printArray("Before sort",a,n);
    qsort(b, n, sizeof(int), cmp);

    start = MPI_Wtime();
    quickSort(a, 0, n - 1);
    end = MPI_Wtime();
    cout << "Time: " << end-start << endl;

    //printArray("After sort",a,n);
    cout << isArrayEqual(a, b, n) << endl;

    MPI_Finalize();
}
#endif