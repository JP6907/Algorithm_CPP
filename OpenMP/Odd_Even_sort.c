//
// Created by zjp on 18-12-13.
//
/**
 * 使用openMP实现奇偶归并排序
 * 外部循环存在循环依赖
 * 内部循环不存在任何循环依赖，可并行化
 *
 * 第一种方式耗时：24107 毫秒
 * 第二种方式耗时：47377 毫秒
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

void swap(int *a,int *b);
void copyArray(int *dest,int *sour,int num);
void printArray(int a[],int n);
//qsort函数调用的比较函数
int cmp(const void* a,const void* b);
//生成随机数组
void generateRandomArray(int a[],int n);
//验证a是否和verifyArray相同
int verify(int a[],int verifyArray[],int n);

int main(){

    struct timeval start;
    struct timeval end;
    unsigned long diff;
    gettimeofday(&start,NULL);

    int thread_count = 10;

    int n = 1000,phase,i;
    int a[n],b[n];
    generateRandomArray(a,n);
    //b数组用快速排序，用于验证a数组并行排序结果是否正确
    copyArray(b,a,n);
    qsort(b,n,sizeof(int),cmp);


    //pragma在外层循环里面，可能会使得在每一遍外部循环都创建和合并thread_count个线程
    for(phase=0;phase<n;phase++){
        if(phase%2==0){
#pragma omp parallel for num_threads(thread_count) \
    default(none) shared(a,n) private(i)
            for(i=1;i<n;i+=2)
                if(a[i-1]>a[i])
                    swap(&a[i-1],&a[i]);
        }else{
#pragma omp parallel for num_threads(thread_count) \
    default(none) shared(a,n) private(i)
            for(i=1;i<n-1;i+=2){
                if(a[i]>a[i+1])
                    swap(&a[i],&a[i+1]);
            }
        }
    }
     /**
     //可以在外部循环之前创建线程，在每次内部循环重用它们
#pragma omp parallel num_threads(thread_count) \
    default(none) shared(a,n) private(i,phase)
    for(phase=0;phase<n;phase++){
        if(phase%2==0){
#pragma omp for
            for(i=1;i<n;i+=2)
                if(a[i-1]>a[i])
                    swap(&a[i-1],&a[i]);
        }else{
#pragma omp for
            for(i=1;i<n-1;i+=2){
                if(a[i]>a[i+1])
                    swap(&a[i],&a[i+1]);
            }
        }
    }
      */
    gettimeofday(&end,NULL);
    diff = 1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec;
    printf("time:%ld\n",diff);

    printf("%d\n",verify(a,b,n));


    return 0;
}



void swap(int *a,int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void copyArray(int *dest,int *sour,int num){
    int i;
    for(i=0;i<num;i++)
        dest[i] = sour[i];
}
void printArray(int a[],int n){
    int i;
    for(i=0;i<n;i++)
        printf("%d ",a[i]);
    printf("\n");
}

int cmp(const void* a,const void* b){
    return *((int*)a) - *((int*)b);
}
void generateRandomArray(int a[],int n){
    int i;
    for(i=0;i<n;i++)
        a[i] = rand()%n;
}
int verify(int a[],int verifyArray[],int n){
    int i;
    for(i=0;i<n&&a[i]==verifyArray[i];i++);

    return i==n?1:0;

}