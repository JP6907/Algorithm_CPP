//
// Created by zjp on 18-12-6.
//
/**
 * MPI中的派生数据类型
 * MPI_Type_create_struct
 */
#include <stdio.h>
#include <mpi.h>

void Build_mpi_type(double* a,double* b,int* n,MPI_Datatype* input_mpi_type);
void Get_input(int my_rank,int comm_sz, double* a, double* b,int* n);

int main(){

    double *a,*b;
    int *n;
    int my_rank,comm_sz;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);

    Get_input(my_rank,comm_sz,a,b,n);

    MPI_Finalize();
    return 0;
}

void Build_mpi_type(double* a,double* b,int* n,MPI_Datatype* input_mpi_type){
    int array_of_blocklengths[3] = {1,1,1}; //制定每种数据类型中元素的个数
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE,MPI_DOUBLE,MPI_INT};
    MPI_Aint a_addr,b_addr,n_addr;
    MPI_Aint array_of_displayments[3] = {0};  //偏移量数组

    MPI_Get_address(a,&a_addr);//获取地址
    MPI_Get_address(b,&b_addr);
    MPI_Get_address(n,&n_addr);

    array_of_displayments[1] = b_addr - a_addr;//计算偏移量
    array_of_displayments[2] = n_addr - b_addr;

    MPI_Type_create_struct(3,array_of_blocklengths,array_of_displayments,array_of_types,input_mpi_type);
    MPI_Type_commit(input_mpi_type);

}

void Get_input(int my_rank,int comm_sz, double* a, double* b,int* n){
    MPI_Datatype input_mpi_type;
    Build_mpi_type(a,b,n,&input_mpi_type);

    if(my_rank==0){
        printf("Enter a,b,n:\n");
        scanf("%lf %lf %d",a,b,n);
    }
    //为了使用新数据类型，需要在每个进程调用MPI_Bcast
    MPI_Bcast(a,1,input_mpi_type,0,MPI_COMM_WORLD);
    MPI_Type_free(&input_mpi_type);
}