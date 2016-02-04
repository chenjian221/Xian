#ifndef SORT_ALGS
#define SORT_ALGS

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int * bubble_sort_serial(int * unsorted, long num_elems);
int * bubble_sort_parallel(int * unsorted, long num_elems, int threads);

#endif
