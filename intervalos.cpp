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

void mergeSort(intervalo_t *copy, uint left, uint right){ 
  if(left >= right)
    return;

  uint mid = left+(right-left)/2; 
  mergeSort(copy, left, mid); 
  mergeSort(copy, mid+1, right); 

  uint t1 = left;
  uint t2 = mid + 1;
  uint N1 = mid;
  uint N2 = right;
  uint counter = 0;
  intervalo_t *sortedArr = new intervalo_t[right-left+1];

  while(t1 <= N1 && t2 <= N2){
    if(copy[t1].fin <= copy[t2].fin)
      sortedArr[counter++] = copy[t1++];
    else
      sortedArr[counter++] = copy[t2++];
  }

  while(t1 <= N1)
    sortedArr[counter++] = copy[t1++];

  while(t2 <= N2)
    sortedArr[counter++] = copy[t2++];

  for(uint i = 0; i < (right-left+1); i++)
    copy[left++] = sortedArr[i];

  delete[] sortedArr;
} 

intervalo_t *orderer(const intervalo_t *intervalos, uint n){
  intervalo_t *copy = copy_intervals(intervalos, n);
  mergeSort(copy, 0, n-1);
  return copy;
}

bool *max_cantidad(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  intervalo_t *ord = orderer(intervalos, n);
  uint reference = ord[0].fin;
  ret[0] = true;

  for(uint i=1; i < n; i++){
    if(ord[i].inicio < reference)
      ret[i] = false;
    else{
      ret[i] = true;
      reference = ord[i].fin;
    }
  }

  delete[] ord;
  return ret;
}

bool *max_volumen(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  intervalo_t *ord = orderer(intervalos, n);
  uint *weight = new uint[n];
  uint maxIndex = 0;
  weight[0] = ord[0].volumen;
  ret[0] = true;
  uint i, j;
  
  for(i=1; i < n; i++){
    j = i;
    while(j > 0 && ord[j-1].fin > ord[i].inicio)
      j--;

    if(j == 0)
      weight[i] = ord[i].volumen;
    else
      weight[i] = weight[j-1]+ord[i].volumen;

    if(ord[i].volumen > weight[maxIndex] || (weight[j-1]+ord[i].volumen) > weight[maxIndex]){
      maxIndex = i;
      ret[i] = true;

      for(uint x=j; x <= i-1; x++)
        ret[x] = false;
    }else
      ret[i] = false;
  }

  delete[] weight;
  delete[] ord;
  return ret;
}
