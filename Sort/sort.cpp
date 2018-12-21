//
// Created by zjp on 18-12-21.
//
#include "sort.h"

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