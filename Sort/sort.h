//
// Created by zjp on 18-12-21.
//

#ifndef PARALLELPROGRAM_SORT_H
#define PARALLELPROGRAM_SORT_H

/**
 * 秩排序
 * 统计出小于每个被选数的个数，个数值即为被选择的数在序列中的位置
 * @param a
 * @param len
 */
void rankSort(int *a,int len);

/**
 * 快速排序
 * @param a
 * @param low
 * @param high
 */
void quickSort(int *a, int low, int high);
int partion(int *a, int low, int high);

#endif //PARALLELPROGRAM_SORT_H
