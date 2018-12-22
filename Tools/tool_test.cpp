//
// Created by zjp on 18-12-22.
//
#include "catch.h"
#include <string>
#include "Tools.h"

using namespace std;

TEST_CASE("createRandomArrayToFile"){
    SECTION("1") {
        string output_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data.txt";
        createRandomArrayToFile(5000000, 0, 10000000, output_filename);
    }
}

TEST_CASE("createRandomArray_noRepeat"){
    int len = 200;
    int min = 0,max = 100;
    int *num = createRandomArray_noRepeat(len,min,max);
    //printArray("num",num,len);
}

TEST_CASE("createRandomArrayToFile_noRepeat"){
    int len = 10000;
    int min = 0,max = 10000;
    string output_filename = "/home/zjp/Documents/Parallel/ParallelProgram/Sort/data_no_repeat.txt";
    createRandomArrayToFile_noRepeat(len,min,max,output_filename);
}