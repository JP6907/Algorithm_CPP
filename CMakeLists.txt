cmake_minimum_required(VERSION 3.10)
project(ParallelProgram)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. DIR_SRCS)

include_directories(DataStruct Tools catch /usr/local/include)

add_executable(GraphSearch mpi/GraphSearch.cpp DataStruct/Graph.cpp)

target_link_libraries(GraphSearch mpi)

#add_executable(RankSort Sort/RankSort.cpp Tools/Tools.cpp)
#
#target_link_libraries(RankSort mpi)
#
#add_executable(QuickSort Sort/QuickSort.cpp Tools/Tools.cpp)

add_executable(sort_test Sort/sort_test.cpp  Sort/sort.cpp Tools/Tools.cpp Sort/ExternSort.cpp)

add_executable(tool_test Tools/tool_test.cpp Tools/Tools.cpp)

add_executable(struct_test DataStruct/struct_test.cpp)

add_executable(BF BigData/BloomFilter.cpp)


