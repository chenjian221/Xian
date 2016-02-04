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

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "sort.h"
#include <time.h>

void init_array(int * list, long num_elems, unsigned int seed) {
  srand(seed);
  for (long i=0; i < num_elems; i++) {
    list[i] = rand();
  }
}

int validate_sort(int * list, long num_elems) {
  int last = list[0];
  for(long i=1; i < num_elems; i++) {
    if (last > list[i]) {
      return -1;
    }
    last = list[i];
  }
  return 1;
}

int main(int argc, char * argv[]) {
  long num_elems = atol(argv[1]);
  unsigned int seed = atoi(argv[2]);
  int threads = atoi(argv[3]);

  int unsorted[num_elems];
  int * sorted;

  printf("Num Elems: %ld\n",num_elems);
  printf("Seed: %u\n",seed);
  printf("Threads: %d\n",threads);

  printf("\nInitializing Array...");

  init_array(unsorted, num_elems, seed);

  printf("Done\n");
  
  printf("\nSorting...\n\n");


  /* NOTE: ONLY TIME THE CALL TO 
   * bubble_sort_serial() or bubble_sort_parallel()
   */
  double start;
  double end;
  double time_spent;
  double timer[20];
  
  if (threads == 1) {
    time_spent=0;
    for(int count=0;count<20;count++){
    start = omp_get_wtime();
    sorted = bubble_sort_serial(unsorted,num_elems);
    end = omp_get_wtime();
    timer[count]=(double)(end-start);
    time_spent+=(double)(end-start);
    }
    time_spent/=20.0;
  } else {
    time_spent=0;
    for(int count=0;count<20;count++){
    start = omp_get_wtime();
    sorted = bubble_sort_parallel(unsorted,num_elems,threads);
    end = omp_get_wtime();
    timer[count]=(double)(end-start);
    time_spent+=(double)(end-start);
    }
    time_spent/=20.0;
  }
  
  double tmp=0.0;
  for(int i=0;i<20;i++){
      tmp+=pow(timer[i]-time_spent,2);
  }
  tmp=pow(tmp,0.5)/time_spent;
  printf("variance %lg \n",tmp);
  printf("Time Spent %lg \n",time_spent);

  printf("Done\n");

  printf("\nValidating Result...");  
  int valid = validate_sort(sorted,num_elems);
  if (valid == 1) {
    printf("\nSorting valid!\n");
  } else {
    printf("\nINVALID Sorting!\n");
  }
  printf("Done\n");
}
