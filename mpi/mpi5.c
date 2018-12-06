//
// Created by zjp on 18-12-3.
//
/**
 * 统计各进程消耗的墙上时间.
 */
#include <unistd.h>
#include <stdio.h>
#include <mpi.h>

int
main (int argc, char *argv[])
{
    int myrank, nprocs, name_len, flag;
    double start_time, end_time;
    char host_name[10];

    // 检测MPI环境是否已经初始化.
    MPI_Initialized (&flag);
    fprintf (stderr, "flag: %d\n", flag);

    // 初始化MPI环境
    MPI_Init (&argc, &argv);

    // 获取当前进程在通信器MPI_COMM_WORLD中的进程号
    MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

    // 获取通信器MPI_COMM_WORLD中的进程数
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);

    // 获取处理器名称,和名字长度
    MPI_Get_processor_name(host_name, &name_len);
    if (myrank == 0)
    {
        fprintf (stderr, "precision of MPI_Wtime(): %f.\n", MPI_Wtick());  // 计时的精度
        fprintf (stderr, "host name: %s\n", host_name);
    }

    //获取墙上时间
    start_time = MPI_Wtime();
    sleep(myrank * 3);

    //获取墙上时间
    end_time = MPI_Wtime();
    fprintf (stderr, "myrank: %d. I have slept %f seconds.\n",
             myrank, end_time - start_time);

    // 结束MPI环境
    MPI_Finalize ();
    return 0;
}