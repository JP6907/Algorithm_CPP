//
// Created by zjp on 18-12-21.
//
#include "catch.h"
#include "sort.h"
#include "Tools.h"
#include <time.h>

#define RANK_SORTED
#define QUICK_SORTED
#define BITSET_SORTED
#define EXTERN_SORTED
#define HANDSHAKE_SORT

using namespace std;

#ifdef RANK_SORT
TEST_CASE("rankSort"){
    clock_t start_time,end_time;
    int n=10000 ,max=50;
    int *array = createRandomArray(n,max);
    //printArray("Before sort",array,n);

    start_time = clock();
    rankSort(array,n);
    end_time = clock();
    cout << "Time: " << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
    //printArray("After Sort",array,n);

}
#endif


#ifdef QUICK_SORT
///10000个数排序 ，时间 0.00103641
TEST_CASE("quickSort"){
    clock_t start_time,end_time;
    int n = 1000000;
    int max = 1000000;
    double start;
    double end;

    int *a = createRandomArray(n, max);
    int *b = new int[n];
    copyArray(b, a, n);
    //printArray("Before sort",a,n);
    qsort(b, n, sizeof(int), cmp);

    start_time = clock();
    quickSort(a, 0, n - 1);
    end_time = clock();
    cout << "Time: " << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;

    //printArray("After sort",a,n);
    cout << isArrayEqual(a, b, n) << endl;

}
#endif

#ifdef BITSET_SORT

TEST_CASE("bitset_sort"){
    clock_t start_time,end_time;
//    string output_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data.txt";
//    createRandomArrayToFile(10, 0, 100, output_filename);

    string intput_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data.txt";
    string intput_filename1 = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data_no_repeat.txt";
    string output_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/sort.txt";
    start_time = clock();
    //bitsetSort_file(100,intput_filename,output_filename);
    bitsetSort_file(10000,intput_filename1,output_filename);
    end_time = clock();
    cout << "Time:" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;
}
#endif

#ifdef EXTERN_SORT

#include "ExternSort.h"
TEST_CASE("externSort"){
    clock_t start_time,end_time;
    string intput_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data.txt";
    string output_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/sort.txt";
    int len = 10000000;
    int min = 0,max = 10000000;
    int each_count = 1000000;
    //createRandomArrayToFile_noRepeat(len,min,max,intput_filename);  //生成75.2M的数据

    start_time = clock();
    ExternSort externSort(intput_filename.data(),output_filename.data(),each_count);
    externSort.setCurrentDirectory("/home/zjp/Documents/Parallel/ParallelProgram/Sort"); //设置临时文件目录
    externSort.sort();
    end_time = clock();
    cout << "Time:" << (double)(end_time-start_time)/CLOCKS_PER_SEC << endl;

}
#endif

#ifdef HANDSHAKE_SORT
TEST_CASE("handShake_sort"){
    int len = 20,max = 100;
    int *a = createRandomArray(len,max);
    printArray("Before sort",a,len);
    mergeSort_handShake(a,0,len-1);
    printArray("After sort",a,len);

}
#endif