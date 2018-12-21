//
// Created by zjp on 18-12-20.
//

#ifndef PARALLELPROGRAM_TOOLS_H
#define PARALLELPROGRAM_TOOLS_H

#include <string>
/**
 * 创建随机数组
 * @param len 长度
 * @param max 可能达到的最大值
 * @return int*
 */
int* createRandomArray(int len,int max);

/**
 * 打印数组
 * @param flag 打印标志
 * @param a 数组
 * @param len 数组长度
 */
void printArray(std::string flag,int* a,int len);
/**
 * 判断两个数组是否相同
 * @param a
 * @param b
 * @param n
 * @return
 */
bool isArrayEqual(int *a,int *b,int n);

/**
 * qsort函数调用的比较函数
 * @param a
 * @param b
 * @return
 */
int cmp(const void* a,const void* b);

/**
 * 复制数组
 * @param dest
 * @param sour
 * @param num
 */
void copyArray(int *dest,int *sour,int num);

/**
 * 以2为底的num的对数
 * @param num
 * @return
 */
int log2(int num);

/**
 * 求2的n次幂
 * @param n
 * @return
 */
int exp2(int n);

#endif //PARALLELPROGRAM_TOOLS_H
