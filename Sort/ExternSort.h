//
// Created by zjp on 18-12-22.
//

#ifndef PARALLELPROGRAM_EXTERNSORT_H
#define PARALLELPROGRAM_EXTERNSORT_H

#include <fstream>
/**
 * 外部归并排序
 */
class ExternSort{
private:
    int m_count;  //每次在内存中排序的数量
    char* m_in_file; //输入文件路径
    char* m_out_file; //输出文件路径
    char* currentDirectory; //当前工作目录，用于存放暂时文件
public:
    /**
     *
     * @param inputFile
     * @param outputFile
     * @param count 每次在内存中排序的数量
     */
    ExternSort(const char* inputFile,const char* outputFile,int count);

    ~ExternSort();

    void sort();
    /**
     * 设置存放暂时文件的目录，最后面 不需要 加斜杠 /
     * @param directory
     */
    void setCurrentDirectory(const char* directory);


protected:
    /**
     * 从文件中读取数据
     * @param f 文件
     * @param a
     * @param n 读取数量
     * @return 实际读取数量
     */
    int readData(FILE* f,int a[],int n);
    /**
     * 写数据到文件
     * @param f
     * @param a
     * @param n
     */
    void writeData(FILE *f,int a[],int n);
    /**
     * 获取临时文件名
     * @param index
     * @return
     */
    char* getTempFileName(int index);
    /**
     * 将文件分块在内存排序
     * @return 分块数量
     */
    int memorySort();
    /**
     * 归并临时文件并输出到文件
     * @param fileCount 临时文件数量
     */
    void mergeSort(int fileCount);
    /**
     * 用于qsort函数调用
     * @param a
     * @param b
     * @return
     */
    static int cmp(const void* a,const void* b);

};
#endif //PARALLELPROGRAM_EXTERNSORT_H
