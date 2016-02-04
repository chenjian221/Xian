/*
  Copyright (c) 2015, Jason Poovey
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "sort.h"

int * bubble_sort_serial(int * unsorted, long num_elems) {
  int * sorted = (int*)calloc(sizeof(int),num_elems);
  for (int i=0; i < num_elems; i++) {
    sorted[i] = unsorted[i];
  }

  int tmp;

  for (long i=0; i < num_elems; i++) {
    for (long j=0; j < num_elems - i; j++) {
      if (sorted[j] > sorted[j+1]) {
        tmp = sorted[j];
        sorted[j] = sorted[j+1];
        sorted[j+1] = tmp;
      }
    }
  }

  return sorted;
}

int * bubble_sort_parallel(int * unsorted, long num_elems, int threads) {
  int * sorted = (int*)calloc(sizeof(int),num_elems);
  for (int i=0; i < num_elems; i++) {
    sorted[i] = unsorted[i];
  }

  int tmp;
  int change = 1;
  long j = 0;
  int tid;
  int nthreads;
  long N = num_elems;


  while (change) {
    change = 0;
    
    #pragma omp parallel private(j,tmp,tid,nthreads) shared(change,sorted,N) num_threads(threads)
    {
      tid = omp_get_thread_num();
      nthreads = omp_get_num_threads();

      long min1 = (long)(((double)tid/nthreads)*N);
      long min2 = (long)(((double)tid/nthreads)*N);
      long max = (long)(((double)(tid+1)/nthreads)*N);
      
      if (min1%2 == 1) {
        min1++;
      }

      if (min2%2 == 0) {
        min2++;
      }

      for (j=min1; j < max; j+=2) {
        if (j==N) continue;
        if (sorted[j] > sorted[j+1]) {
          tmp = sorted[j];
          sorted[j] = sorted[j+1];
          sorted[j+1] = tmp;
          if (change == 0) {
            __sync_fetch_and_add(&change,1);
          }
        }
      }

      #pragma omp barrier

      for (j=min2; j < max; j+=2) {
        if (j==N) continue;
        if (sorted[j] > sorted[j+1]) {
          tmp = sorted[j];
          sorted[j] = sorted[j+1];
          sorted[j+1] = tmp;
          if (change == 0) {
            __sync_fetch_and_add(&change,1);
          }
        }
      }
    }

  }

  return sorted;
}
