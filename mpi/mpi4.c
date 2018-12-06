//
// Created by zjp on 18-12-3.
//
/**
 * 多个进程向同一进程发送消息时，不同的消息到达目的进程的顺序不确定。
 * 此时，可以通过不同的标签来识别来自不同进程的消息.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1

int
main (int argc, char *argv[])
{
    int myrank, dest;
    int my_int[N], int_from_1[N], int_from_2[N];
    MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

    my_int[0] = myrank * 111;

    if (myrank == 1)
    {
        dest = 0;
        MPI_Send (my_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD);
    }
    else if (myrank == 2)
    {
        dest = 0;
        MPI_Send (my_int, N, MPI_INT, dest, 99, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv (int_from_1, N, MPI_INT,
                  MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        //接收者可以给source指定一个任意值MPI_ANY_SOURCE
        //,标识任何进程发送的消息都可以接受，即本接收操作
        //可以匹配任何进程发送的消息，但其它的要求还必须满足，比如tag的匹配。
        MPI_Recv (int_from_2, N, MPI_INT,
                  MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        printf ("myrank %d: int_from_1 = %d, \tint_from_2 = %d\n", myrank,
                int_from_1[0], int_from_2[0]);
    }
//与前不同，由于1号进程和2号进程发消息的速度指不定谁快，所以在消息tag一样的情况的下，接收具有随机性。
//从这个程序进一步体会tag在消息传递中的作用。
    MPI_Finalize ();
    return 0;
}
