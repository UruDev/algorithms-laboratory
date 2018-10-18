/* 5008417 */

#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>

intervalo_t *copy_intervals(const intervalo_t *intervals, uint len) {
  intervalo_t *copy = new intervalo_t[len];
  for (uint i=0; i < len; i++)
    copy[i] = intervals[i];
  return copy;
}

void merger(intervalo_t *copy, uint left, uint mid, uint right, uint *origins){
  uint t1 = left;
  uint t2 = mid + 1;
  uint N1 = mid;
  uint N2 = right;
  uint counter = 0;
  intervalo_t *sortedArr = new intervalo_t[right-left+1];

  while(t1 <= N1 && t2 <= N2){
    if(copy[t1].fin <= copy[t2].fin){
      sortedArr[counter++] = copy[t1++];
      origins[counter-1] = t1-1;
    }else{
      sortedArr[counter++] = copy[t2++];
      origins[counter-1] = t2-1;
    }
  }

  while(t1 <= N1){
    sortedArr[counter++] = copy[t1++];
    origins[counter-1] = t1-1;
  }

  while(t2 <= N2){
    sortedArr[counter++] = copy[t2++];
    origins[counter-1] = t2-1;
  }

  uint len = right-left+1;
  for(uint i = 0; i < len; i++)
    copy[left++] = sortedArr[i];

  delete[] sortedArr;
}

void mergeSort(intervalo_t *copy, uint left, uint right, uint *origins){ 
  if(left >= right)
    return;

  uint mid = left+(right-left)/2; 
  mergeSort(copy, left, mid, origins); 
  mergeSort(copy, mid+1, right, origins); 
  merger(copy, left, mid, right, origins);
} 

intervalo_t *orderer(const intervalo_t *intervalos, uint n, uint *origins){
  intervalo_t *copy = copy_intervals(intervalos, n);
  for(uint p=0; p < n; p++)
    origins[p] = 0;
  mergeSort(copy, 0, n-1, origins);
  return copy;
}

bool *max_cantidad(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  uint *origins = new uint[n];
  intervalo_t *ord = orderer(intervalos, n, origins);
  uint reference = ord[origins[0]].fin;
  ret[origins[0]] = true;
  for(uint q=0; q<n; q++)
    printf("ord> %d | origin> %d\n", ord[q].fin, origins[q]);

  for(uint i=1; i < n; i++){

    if(ord[i].inicio < reference)
      ret[origins[i]] = false;
    else{
      ret[origins[i]] = true;
      reference = ord[i].fin;
    }
  }

 // for(uint q=0; q<n; q++)
 //   printf("fin> %d | ret[%d] = %d\n", ord[q].fin, q, ret[q]);
  delete[] ord;
  delete[] origins;
  return ret;
}

bool *max_volumen(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  uint *origins = new uint[n];
  intervalo_t *ord = orderer(intervalos, n, origins);

  uint *weight = new uint[n];
  uint maxIndex = 0;
  weight[0] = ord[0].volumen;
  ret[origins[0]] = true;
  uint i, j;
  
  for(i=1; i < n; i++){
    j = i;
    while(j > 0 && ord[j-1].fin > ord[i].inicio)
      j--;

    if(j == 0){
      weight[i] = ord[i].volumen;
    }else
      weight[i] = weight[j-1]+ord[i].volumen;

    if(ord[i].volumen > weight[maxIndex] || (j > 0 && (weight[j-1]+ord[i].volumen) > weight[maxIndex])){
      maxIndex = i;
      ret[origins[i]] = true;

      for(uint x=j; x <= i-1; x++)
        ret[origins[x]] = false;
    }else
      ret[origins[i]] = false;
  }

  delete[] origins;
  delete[] weight;
  delete[] ord;
  return ret;
}
