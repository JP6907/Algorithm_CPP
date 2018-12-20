cmake_minimum_required(VERSION 3.10)
project(ParallelProgram)

set(CMAKE_CXX_STANDARD 14)

#add_executable(ParallelProgram main.c mpi/mpi2.c)

#add_executable(MPI1 mpi/mpi1.c mpi/mpi3.c mpi/mpi4.c mpi/mpi5.c mpi/mpi6.c mpi/CalculateSum.c)


#target_link_libraries(ParallelProgram mpi)

#target_link_libraries(MPI1 mpi)

#add_executable(TrapezoidArea mpi/3-2TrapezoidArea.c mpi/3-9ScatterVector.c mpi/3-12Mat_vect_mult.c mpi/3-13CreateType.c mpi/3-15Odd_Even_sort.c mpi/3-15Odd_Even_sort.c mpi/Tree_Sum.c mpi/Tree_Sum2.c mpi/Butterfly_Sum3.c mpi/Tree_Sum3.c mpi/PI.c mpi/Mat_Mult.c Pthreads/HelloWorld.c Pthreads/Mat_Mult.c)

#target_link_libraries(TrapezoidArea mpi)

#include_directories(OpenMP)
#
#add_executable(main main.c)
#
#target_link_libraries(main pthread)
#
#add_executable(MessageQueue_Parallel OpenMP/MessageQueue_Parallel.c OpenMP/MessageQueue.c)
#
#target_link_libraries(MessageQueue_Parallel omp)

include_directories(DataStruct Tools /usr/local/include)

add_executable(GraphSearch mpi/GraphSearch.cpp DataStruct/Graph.cpp)

target_link_libraries(GraphSearch mpi)

add_executable(RankSort Sort/RankSort.cpp Tools/Tools.cpp)

target_link_libraries(RankSort mpi)
