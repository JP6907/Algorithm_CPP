//
// Created by zjp on 18-12-22.
//

#include "ExternSort.h"
#include "LoserTree.h"
#include <iostream>
#include <assert.h>
#include <cstring>

using namespace std;

ExternSort::ExternSort(const char *inputFile, const char *outputFile, int count) {
    m_count = count;
    m_in_file = new char[strlen(inputFile)+1];
    strcpy(m_in_file,inputFile);
    m_out_file = new char[strlen(outputFile)+1];
    strcpy(m_out_file,outputFile);
}

ExternSort::~ExternSort() {
    delete []m_in_file;
    delete []m_out_file;
    delete []currentDirectory;
}

void ExternSort::setCurrentDirectory(const char *directory) {
    currentDirectory = new char[strlen(directory)+1];
    strcpy(currentDirectory,directory);
}

void ExternSort::sort() {
    //将文件分块在内存中排序，并写入到临时文件中
    int file_count = memorySort();
    //归并临时文件并输出到文件
    mergeSort(file_count);
}

int ExternSort::readData(FILE *f, int *a, int n) {
    int i;
    for(i=0;i<n && (fscanf(f,"%d",&a[i])!=EOF);i++);

    cout << "Read " << i << " integer" << endl;
    return i;
}

void ExternSort::writeData(FILE *f, int *a, int n) {
    for(int i=0;i<n;i++)
        fprintf(f,"%d ",a[i]);
}

char* ExternSort::getTempFileName(int index) {
    char* tempFile = new char[100];
    sprintf(tempFile,"%s/temp%d.txt",currentDirectory,index);
    return tempFile;
}

int ExternSort::memorySort() {
    FILE* fin = fopen(m_in_file,"rt");
    int n=0, fileCount = 0;
    int* array = new int[m_count];
    //每次读取m_count个数在内存中做一次排序，并写入临时文件
    //n为实际读取的数量
    while((n=readData(fin,array,m_count))>0){
        qsort(array,n,sizeof(int),cmp);
        char* fileName = getTempFileName(fileCount++);
        FILE *tempFile = fopen(fileName,"w");
        assert(tempFile);
        delete fileName;
        writeData(tempFile,array,n);
        fclose(tempFile);
    }
    delete []array;
    fclose(fin);
    return fileCount;
}

void ExternSort::mergeSort(int fileCount) {
    if(fileCount<=0)
        return;
    const int outputBufferSize = 10000;
    int w = 0;
    int* outputBuffer = new int[outputBufferSize];  // 输出缓存

    FILE* fout = fopen(m_out_file,"w");
    assert(fout);
    FILE** fArray = new FILE*[fileCount];
    //打开文件
    for(int i=0;i<fileCount;i++){
        char* tempFile = getTempFileName(i);
        fArray[i] = fopen(tempFile,"rt");
        assert(fArray[i]);
        delete tempFile;
    }
    //储存每个文件当前读取的数字
    int* data = new int[fileCount];
    //标志文件是否读完
    bool* isFinish = new bool[fileCount];

    memset(data,0,sizeof(int)*fileCount);
    memset(isFinish, false,sizeof(bool)*fileCount);

    //读取每个文件第一个数到data数组中
    for(int i=0;i<fileCount;i++){
        if(fscanf(fArray[i],"%d",&data[i])==EOF)
            isFinish[i] = true;
    }
    //归并
    while(true){
        //记录data数组中的最小值及其索引
        int min;
        int index = 0;
        //寻找到第一个未读完的文件
        while(index<fileCount&&isFinish[index])
            index ++;

        if(index>=fileCount)  //已经读完
            break;

        //寻找data[]中最小值
        min = data[index];
        for(int i=index+1;i<fileCount;i++){
            if(!isFinish[i]&&data[i]<min){
                min = data[i];
                index = i;
            }
        }
        //每次将data[]记录的最小值写到文件
        //并判断最小值所在文件是否已经读取完毕
        if(fscanf(fArray[index],"%d",&data[index])==EOF)
            isFinish[index] = true;
        //输出最小值
        //fprintf(fout,"%d ",min);   //5.63771 s
        //使用输出缓存                 //5.2726 s
        outputBuffer[w++] = min;
        if(w==outputBufferSize) {
            writeData(fout, outputBuffer, w);
            w = 0;
        }
    }
    delete []outputBuffer;
    delete []isFinish;
    delete []data;
    for(int i=0;i<fileCount;i++)
        fclose(fArray[i]);
    delete []fArray;
    fclose(fout);
}

void ExternSort::mergeSort_loserTree(int fileCount) {
    if(fileCount<=0)
        return;
    const int outputBufferSize = 10000;
    int w = 0;
    int* outputBuffer = new int[outputBufferSize];  // 输出缓存

    FILE* fout = fopen(m_out_file,"w");
    assert(fout);
    FILE** fArray = new FILE*[fileCount];
    //打开文件
    for(int i=0;i<fileCount;i++){
        char* tempFile = getTempFileName(i);
        fArray[i] = fopen(tempFile,"rt");
        assert(fArray[i]);
        delete tempFile;
    }
    int temp;
    //读取每个文件第一个数到data数组中
    for(int i=0;i<fileCount;i++){
        if(fscanf(fArray[i],"%d",&temp)==EOF)

    }

    LoserTree* loserTree = new LoserTree(fileCount,data);
    while(true){
        int index = 0;
        //找到第一个未读完的文件
        while(index<fileCount&&isFinish[index])
            index++;
        if(index>=fileCount)
            break;

        outputBuffer[w++] = loserTree->getMin();
        if(w==outputBufferSize) {
            writeData(fout, outputBuffer, w);
            w = 0;
        }

        if(fscanf(fArray[index],"%d",&data[index])==EOF)
            isFinish[index] = true;
    }
    //归并
    while(true){
        //记录data数组中的最小值及其索引
        int min;
        int index = 0;
        //寻找到第一个未读完的文件
        while(index<fileCount&&isFinish[index])
            index ++;

        if(index>=fileCount)  //已经读完
            break;

        //寻找data[]中最小值
        min = data[index];
        for(int i=index+1;i<fileCount;i++){
            if(!isFinish[i]&&data[i]<min){
                min = data[i];
                index = i;
            }
        }
        //每次将data[]记录的最小值写到文件
        //并判断最小值所在文件是否已经读取完毕
        if(fscanf(fArray[index],"%d",&data[index])==EOF)
            isFinish[index] = true;
        //输出最小值
        //fprintf(fout,"%d ",min);   //5.63771 s
        //使用输出缓存                 //5.2726 s
        outputBuffer[w++] = min;
        if(w==outputBufferSize) {
            writeData(fout, outputBuffer, w);
            w = 0;
        }
    }
    delete []outputBuffer;
    delete []isFinish;
    delete []data;
    for(int i=0;i<fileCount;i++)
        fclose(fArray[i]);
    delete []fArray;
    fclose(fout);
}

int ExternSort::cmp(const void* a,const void* b){
    return *((int*)a) - *((int*)b);
}
