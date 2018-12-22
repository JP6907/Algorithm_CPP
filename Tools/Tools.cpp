//
// Created by zjp on 18-12-20.
//

#include "Tools.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int* createRandomArray(int len,int max){
    int *a = new int[len];
    for(int i=0;i<len;i++)
        a[i] = rand()%(max+1);
    return a;
}

int* createRandomArray_noRepeat(int len,int min,int max){
    //assert(max-min+1>=len);
//    if(max-min+1<len)
//        return NULL;

    srand((int)time(0));
    int rangeSize = max-min+1;
    int *num = new int[rangeSize];
    int *temp = new int[rangeSize];
    for(int i=0;i<rangeSize;i++)
        temp[i] = min+i;
    for(int i=0;i<rangeSize;i++){
        int a = rand()%rangeSize;
        int b = rand()%rangeSize;
            swap(&temp[a],&temp[b]);
    }
    int i;
    for(i=0;i<rangeSize;i++)
        num[i] = temp[i];
    //超出范围，最后都用0补充
    for(;i<len;i++)
        num[i] = 0;

    ///delete []temp;        //出错？？？？
    return num;
}

void createRandomArrayToFile(int len,int min,int max,std::string output_filename){
    srand((int)time(0));
    FILE *fp_output_file = fopen(output_filename.data(),"w");
    assert(fp_output_file);
    for(int i=0;i<len;i++) {
        int x = rand() % (max - min + 1) + min;
        fprintf(fp_output_file,"%d ",x);
    }
    //fprintf(fp_output_file,"%d",EOF);
    fclose(fp_output_file);
}

void createRandomArrayToFile_noRepeat(int len,int min,int max,std::string output_filename){
    int *num = createRandomArray_noRepeat(len,min,max);
    FILE *fp_output_file = fopen(output_filename.data(),"w");
    assert(fp_output_file);
    for(int i=0;i<len;i++){
        fprintf(fp_output_file,"%d ",num[i]);
    }
    fclose(fp_output_file);
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

int log2(int num){
    int i=1,j=2;
    while(j<num){
        j *=2;
        i++;
    }
    if(j>num)
        i--;
    return i;
}


int exp2(int n){
    int i=1;
    while(n>0){
        n--;
        i *=2;
    }
    return i;
}


void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}