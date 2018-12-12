//
// Created by zjp on 18-12-12.
//
/**
 * 有的编译器可能不支持openmp
 * 最好检查 _OPENMP 宏定义是否存在来判断编译器是否支持
 */
#include <stdlib.h>
#include <stdio.h>

#ifdef _OPENMP
    #include <omp.h>
#endif

void Hello(void);

int main(int argc,char* argv[]){
    int thread_count = strtol(argv[1],NULL,10);

#pragma omp parallel num_threads(thread_count)
    Hello();

    return 0;
}

void Hello(void){
#ifdef _OPENMP
        int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
#else
        int my_rank = 0;
        int thread_count = 1;
#endif


    printf("Hello from %d of %d\n",my_rank,thread_count);
}
