/* 5008417 */

#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>

struct set{
  intervalo_t interval;
  uint origin;
};

struct weight{
  uint vol;
  uint depend;
  bool excluded;
};

set *copy_intervals(const intervalo_t *intervals, uint len) {
  set *copy = new set[len];
  for(uint i=0; i < len; i++){
    copy[i].interval = intervals[i];
    copy[i].origin = i;
  }
  return copy;
}

void merger(set *copy, uint left, uint mid, uint right){
  uint t1 = left;
  uint t2 = mid + 1;
  uint N1 = mid;
  uint N2 = right;
  uint counter = 0;
  set *sortedArr = new set[right-left+1];

  while(t1 <= N1 && t2 <= N2){
    if(copy[t1].interval.fin <= copy[t2].interval.fin)
      sortedArr[counter++] = copy[t1++];
    else
      sortedArr[counter++] = copy[t2++];
  }

  while(t1 <= N1)
    sortedArr[counter++] = copy[t1++];

  while(t2 <= N2)
    sortedArr[counter++] = copy[t2++];

  uint len = right-left+1;
  for(uint i = 0; i < len; i++)
    copy[left++] = sortedArr[i];

  delete[] sortedArr;
}

void mergeSort(set *copy, uint left, uint right){ 
  if(left >= right)
    return;

  uint mid = left+(right-left)/2; 
  mergeSort(copy, left, mid); 
  mergeSort(copy, mid+1, right); 
  merger(copy, left, mid, right);
} 

set *orderer(const intervalo_t *intervalos, uint n){
  set *copy = copy_intervals(intervalos, n);
  mergeSort(copy, 0, n-1);
  return copy;
}

bool *max_cantidad(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  set *ord = orderer(intervalos, n);
  uint reference = ord[0].interval.fin;
  ret[ord[0].origin] = true;

  for(uint i=1; i < n; i++){
    if(ord[i].interval.inicio < reference)
      ret[ord[i].origin] = false;
    else{
      ret[ord[i].origin] = true;
      reference = ord[i].interval.fin;
    }
  }

  delete[] ord;
  return ret;
}

bool *max_volumen(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  set *ord = orderer(intervalos, n);

  weight *W = new weight[n];
  uint maxIndex = 0;
  W[0].vol = ord[0].interval.volumen;
  W[0].depend = 0;
  W[0].excluded = false;
  ret[ord[0].origin] = true;
  uint i, j;
  
  for(i=1; i < n; i++){
    j = i;
    while(j > 0 && ord[j-1].interval.fin > ord[i].interval.inicio)
      j--;

    if(j > maxIndex+1)
      j = maxIndex+1;

    if(ord[i].interval.volumen > W[maxIndex].vol || (j > 0 && W[j-1].vol+ord[i].interval.volumen > W[maxIndex].vol)){
      W[i].excluded = false;
      if(j == 0){
        W[i].vol = ord[i].interval.volumen;
        W[i].depend = i;
      }else{
        uint x = j-1;
        while(W[x].excluded)
          x = W[x].depend;
        W[i].depend = x;
        W[i].vol = W[j-1].vol+ord[i].interval.volumen;
      }
    }else{
        W[i].vol = W[maxIndex].vol;
        W[i].depend = maxIndex;
        W[i].excluded = true;
    }

    if(W[i].vol > W[maxIndex].vol){
      maxIndex = i;
      ret[ord[i].origin] = true;

      for(uint x=W[i].depend+1; x <= i-1; x++)
        ret[ord[x].origin] = false;
    }else
      ret[ord[i].origin] = false;

    printf("weight[%d] = %d | depend = %d\n", i, W[i].vol, W[i].depend);
  }

  for(uint q=0; q<=i; q++)
    printf("ret[%d] = %d\n", q, ret[q]);
  delete[] W;
  delete[] ord;
  return ret;
}

