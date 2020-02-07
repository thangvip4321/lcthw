#ifndef _DARRAY_ALGOS_H
#define _DARRAY_ALGOS_H
#include <lcthw/darray.h>
typedef int (* compare_DA)(const void *a,const void *b);
#define DArray_sort(array,cmp,xsort) (xsort(array,cmp))
int DArray_qsort(DArray *array,compare_DA cmp);
int DArray_heapsort(DArray *array,compare_DA cmp);
int DArray_mergesort(DArray *array,compare_DA cmp);
#endif