//
// Created by zjp on 18-12-3.
//
/**
 * tag标签在消息传递中的作用
 * 相同的"源"和"目的"之间, 不同的消息可以通过不同的标签来识别.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1

int main (int argc, char *argv[])
{
    int myrank, dest;
    int my_int[N], get_int[N];
    //要接收，有个MPI_Status类型的status是必要的。
    MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

    my_int[0] = myrank;
    get_int[0] = myrank + 1;
    dest = (myrank == 0) ? (1) : (0);//0给1,1给0。

#if 1
//0号进程传输0和1给1号进程
    if (myrank == 0)
    {
        MPI_Send (my_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD);
        MPI_Send (get_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD);
    }
    else
    {
        // 1号进程的1和2位置分别接收来自0号进程的0和1，如何对应呢？可以动手一试。tag相同，一般按先后顺序发收。
        printf ("myrank %d: my_int = %d, \tget_int = %d\n", myrank,
                my_int[0], get_int[0]);
        MPI_Recv (get_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD, &status);
        MPI_Recv (my_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD, &status);
        printf ("myrank %d: my_int = %d, \tget_int = %d\n", myrank,
                my_int[0], get_int[0]);
    }
#endif

    MPI_Finalize ();
    return 0;
}
