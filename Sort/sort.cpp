//
// Created by zjp on 18-12-21.
//
#include "sort.h"
#include "Tools.h"
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <bitset>

using namespace std;

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

void quickSort(int *a, int low, int high) {
    if (low < high) {
        int pivotpos = partion(a, low, high);
        quickSort(a, low, pivotpos - 1);
        quickSort(a, pivotpos + 1, high);
    }
}

int partion(int *a, int low, int high) {
    int pivot = a[low];
//    srand(int(time(0)));
//    int pivotpos = rand()%(high-low)+low;  //随机选取
//    if(pivotpos!=low)                    //交换，保持后面代码一致
//        swap(a+low,a+pivotpos);
//    int pivot = a[low];

    while (low < high) {
        while (low < high && a[high] >= pivot)
            --high;
        a[low] = a[high];
        while (low < high && a[low] <= pivot)
            low++;
        a[high] = a[low];
    }
    a[low] = pivot;
    return low;
}

void bitsetSort_file(const int maxNum,std::string unsort_filename,std::string sort_filename){
    const int max_each_scan = 5000000;
    bitset<max_each_scan> bit_map;
    bit_map.reset();

    FILE *fp_unsort_file = fopen(unsort_filename.data(),"r");
    assert(fp_unsort_file);
    int num;
    //分两次读入，第一次读入小于 max_each_scan 的数
    ///分两次读入，减少bit_map空间消耗
    while(fscanf(fp_unsort_file,"%d",&num)!=EOF){
        if(num<max_each_scan)
            bit_map.set(num,1);
    }

    FILE *fp_sort_file = fopen(sort_filename.data(),"w");
    assert(fp_sort_file);

    for(int i=0;i<max_each_scan;i++){
        if(bit_map[i]==1)
            fprintf(fp_sort_file,"%d ",i);
    }
    //第二次排序 max_each_scan到2×max_each_scan的数据
    if(maxNum>max_each_scan){
        //文件指针重新定位到开头
        int result = fseek(fp_unsort_file,0,SEEK_SET);
        if(result)
            cout << "fseek failed!" << endl;
        else{
            bit_map.reset();
            while(fscanf(fp_unsort_file,"%d",&num)!=EOF){
                if(num>=max_each_scan&&num<2*max_each_scan){
                    num -= max_each_scan;
                    bit_map.set(num,1);
                }
            }
            for(int i=0;i<max_each_scan;i++){
                if(bit_map[i]==1)
                    fprintf(fp_sort_file,"%d ",i+max_each_scan);
            }
        }
    }
    fclose(fp_sort_file);
    fclose(fp_unsort_file);
}