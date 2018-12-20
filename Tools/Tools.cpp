//
// Created by zjp on 18-12-20.
//

#include "Tools.h"
#include <iostream>
using namespace std;

int* createRandomArray(int len,int max){
    int *a = new int[len];
    for(int i=0;i<len;i++)
        a[i] = rand()%(max+1);
    return a;
}

void printArray(string flag,int* a,int len){
    cout << flag << ": ";
    for(int i=0;i<len;i++)
        cout << a[i] << " ";
    cout << endl;
}

bool isArrayEqual(int *a,int *b,int n){
    int i;
    for(i=0;i<n;i++) {
        if (a[i] != b[i])
            break;
    }
     return i==n;
}

int cmp(const void* a,const void* b){
    return *((int*)a) - *((int*)b);
}

void copyArray(int *dest,int *sour,int num){
    int i;
    for(i=0;i<num;i++)
        dest[i] = sour[i];
}