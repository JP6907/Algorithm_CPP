//
// Created by zjp on 18-12-6.
//
/**
 * 奇偶交换排序算法
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void swap(int *a,int *b);
void copyArray(int *dest,int *sour,int num);
void printArray(int a[],int n);
//qsort函数调用的比较函数
int cmp(const void* a,const void* b);
//生成随机数组
void generateRandomArray(int a[],int n);
//获取合并后的较小值
void Merge_low(int local_keys[],int rec_keys[],int local_n);
//获取合并后的较大值
void Merge_high(int local_keys[],int rec_keys[],int local_n);
//计算配对进程
int Calcul_partner(int phase,int my_rank,int comm_sz);

void Odd_even_sort(int a[],int n);
void Odd_even_sort_Parallel(int a[],int n,int verifyResult[]);


int main(){
    int n = 10000;
    int a[n],b[n];
    generateRandomArray(a,n);
    //b数组用快速排序，用于验证a数组并行排序结果是否正确
    copyArray(b,a,n);
    qsort(b,n,sizeof(int),cmp);
    Odd_even_sort_Parallel(a,n,b);
    //printArray(a,n);

    return 0;
}

//函数调用的方法，每个进程都能获取到参数里面的数据，不要用这种方式，直接在main里面写
//MPI_Scatter 和 MPI_Gather要在所有进程中调用
void Odd_even_sort_Parallel(int keys[],int n,int verifyResult[]){
    int my_rank,comm_sz;
    int local_n,phase,partner,i,j;
    int *local_keys,*recv_keys;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    local_n = n/comm_sz;
    local_keys = (int*)malloc(sizeof(int)*local_n);
    recv_keys = (int*)malloc(sizeof(int)*local_n);

    //scatter效果等同于下面逐个进程send和recev
    MPI_Scatter(keys,local_n,MPI_INT,local_keys,local_n,MPI_INT,0,MPI_COMM_WORLD);
//    if(my_rank==0){
//        //分发数据到各个进程
//        //MPI_Scatter(keys,local_n,MPI_INT,local_keys,local_n,MPI_INT,0,MPI_COMM_WORLD);
//        copyArray(local_keys,keys,local_n);
//        for(i=1;i<comm_sz;i++)
//            MPI_Send(keys+i*local_n,local_n,MPI_INT,i,3,MPI_COMM_WORLD);
//    }else{
//        MPI_Recv(local_keys,local_n,MPI_INT,0,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//        //printf("rank%d local_keys:\n",my_rank);
//        //printArray(local_keys,local_n);
//    }
    //sort local_keys
    qsort(local_keys,local_n,sizeof(int),cmp);
    //exchange
    ///要经过多少个阶段？？？？？？？？？？？？？？？？？？？？？？？
    for(phase=0;phase<comm_sz;phase++){
        partner = Calcul_partner(phase,my_rank,comm_sz);
        if(partner!=MPI_PROC_NULL) {
            /// send flag   recv glag 可以相同??????????????????????????????????
            MPI_Sendrecv(local_keys, local_n, MPI_INT, partner, 0, recv_keys, local_n, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (my_rank<partner)
                Merge_low(local_keys,recv_keys,local_n);
            else
                Merge_high(local_keys,recv_keys,local_n);
        }
    }
    //聚集各个进程的数据
    //MPI_Gather效果同下面逐个进程send 和rece
    MPI_Gather(local_keys,local_n,MPI_INT,keys,local_n,MPI_INT,0,MPI_COMM_WORLD);
    if(my_rank==0){
        for(i=0;i<n;i++)
            if(keys[i]!=verifyResult[i])
                break;
        if(i==n)
            printf("True!!!\n");
        else
            printf("Error!!! i=%d\n",i);
    }
//    if(my_rank!=0)
//        MPI_Send(local_keys,local_n,MPI_INT,0,2,MPI_COMM_WORLD);
//    else{
//        copyArray(keys,local_keys,local_n);
//        for(i=1;i<comm_sz;i++){
//            MPI_Recv(local_keys,local_n,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//            copyArray(keys+i*local_n,local_keys,local_n);
//        }
//        for(i=0;i<n;i++)
//            if(keys[i]!=verifyResult[i])
//                break;
//        if(i==n)
//            printf("True!!!\n");
//        else
//            printf("Error!!! i=%d\n",i);
//    }
    MPI_Finalize();
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