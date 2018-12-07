//
// Created by zjp on 18-12-6.
//
/**
 * 奇偶交换排序算法
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void swap(int *a,int *b);
void printArray(int a[],int n);

//获取合并后的较小值
void Merge_low(int local_keys[],int rec_keys[],int local_n);
//获取合并后的较大值
void Merge_high(int local_keys[],int rec_keys[],int local_n);
//计算配对进程
int Calcul_partner(int phase,int my_rank,int comm_sz);

void Odd_even_sort(int a[],int n);
void Odd_even_sort_Parallel(int a[],int n);


int main(){
    int a[] = {4,5,3,1,8,0};
    Odd_even_sort(a,6);
    printArray(a,6);

    int b[] = {1,3,5,7,9};
    int c[] = {2,4,6,8,10};
    Merge_high(b,c,5);
    printArray(b,5);

    printf("%d\n",Calcul_partner(0,0,5));
    printf("%d\n",Calcul_partner(0,2,5));
    printf("%d\n",Calcul_partner(0,1,5));
    printf("%d\n",Calcul_partner(1,0,5));
    printf("%d\n",Calcul_partner(1,1,5));
    printf("%d\n",Calcul_partner(1,2,5));
    printf("%d\n",MPI_PROC_NULL);
    return 0;
}

void Odd_even_sort_Parallel(int a[],int n){
    int my_rank,comm_sz;
    int local_n,phase,i,j;
    int *local_keys;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    local_n = n/comm_sz;
    local_keys = (int*)malloc(sizeof(int)*local_n);

    if(my_rank==0){
        //分发数据到各个进程

    }
    //sort local_keys

    //exchange

    //聚集各个进程的数据

    MPI_Finalize();
}

void swap(int *a,int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void printArray(int a[],int n){
    int i;
    for(i=0;i<n;i++)
        printf("%d ",a[i]);
    printf("\n");
}

//获取合并后的较小值
void Merge_low(int local_keys[],int rec_keys[],int local_n){
    int temp[local_n];
    int li=0,ri=0,ti=0;
    while(ti<local_n){
        if(local_keys[li]<rec_keys[ri])
            temp[ti++] = local_keys[li++];
        else
            temp[ti++] = rec_keys[ri++];
    }
    for(li=0;li<local_n;li++)
        local_keys[li] = temp[li];
}

//获取合并后的较大值
void Merge_high(int local_keys[],int rec_keys[],int local_n){
    int temp[local_n];
    int li=local_n-1,ri=local_n-1,ti=local_n-1;
    while(ti>=0){
        if(local_keys[li]>rec_keys[ri])
            temp[ti--] = local_keys[li--];
        else
            temp[ti--] = rec_keys[ri--];
    }
    for(li=0;li<local_n;li++)
        local_keys[li] = temp[li];
}

//计算配对进程
int Calcul_partner(int phase,int my_rank,int comm_sz){
    int partner = 0;
    if(phase%2==0){
        if(my_rank%2!=0)
            partner = my_rank-1;
        else
            partner = my_rank+1;
    }else{
        if(my_rank%2!=0)
            partner = my_rank+1;
        else
            partner = my_rank-1;
    }
    if(partner==-1||partner==comm_sz)
        partner = MPI_PROC_NULL;   //空闲进程，将它作为源进程或目的进程，调用通信函数后会直接返回，不会产生任何通信

    return partner;
}


void Odd_even_sort(int a[],int n){
    int phase = 0;
    int i;
    for(phase=0;phase<n;phase++){
        if(phase%2==0){ //偶阶段
            for(i=1;i<n;i+=2)
                if(a[i-1]>a[i])
                    swap(&a[i],&a[i-1]);
        }else{ //奇阶段
            for(i=1;i<n-1;i+=2)
                if(a[i]>a[i+1])
                    swap(&a[i],&a[i+1]);
        }
    }
}