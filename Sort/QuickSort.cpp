//
// Created by zjp on 18-12-20.
//
#include "Tools.h"
#include <iostream>

using namespace std;

void quickSort(int *a, int low, int high);

int partion(int *a, int low, int high);


int main() {
    int n = 10;
    int max = 100;
    int *a = createRandomArray(n, max);
    int *b = new int[n];
    copyArray(b, a, n);
    qsort(b, n, sizeof(int), cmp);
    printArray("b", b, n);
    quickSort(a, 0, n - 1);
    cout << isArrayEqual(a, b, n) << endl;

    cout << log2(7) << endl;
    cout << exp2(3) << endl;
    return 0;
}

void quickSort(int *a, int low, int high) {
    if (low < high) {
        int pivotpos = partion(a, low, high);
        quickSort(a, low, pivotpos - 1);
        quickSort(a, pivotpos + 1, high);
    }
}

int partion(int *a, int low, int high) {
    int pivot = a[low];
    while (low < high) {
        while (low < high && a[high] >= pivot)
            --high;
        a[low] = a[high];
        while (low < high && a[low] <= pivot)
            low++;
        a[high] = a[low];
    }
    a[low] = pivot;
    return low;
}
