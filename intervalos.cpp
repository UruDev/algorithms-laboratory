/* 5008417 */

#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>

struct set{
  intervalo_t interval;
  uint origin;
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

uint *weight(set *ord, uint n){
  uint *values = new uint[n]; 
  ord[0].depend = 0;
  ord[0].excluded = false;
  values[0] = ord[0].interval.volumen; 
  uint i, j;
  
  for(i=1; i < n; i++){
    j = i;
    while(j > 0 && ord[j-1].interval.fin > ord[i].interval.inicio)
      j--;

    if(ord[i].interval.volumen > values[i-1] || (j > 0 && values[j-1]+ord[i].interval.volumen > values[i-1])){
      ord[i].excluded = false;
      if(j == 0){
        values[i] = ord[i].interval.volumen;
        ord[i].depend = i;
      }else{
        uint x = j-1;

        while(ord[x].excluded)
          x = ord[x].depend;

        ord[i].depend = x;
        values[i] = values[j-1] + ord[i].interval.volumen;
      }
    }else{
      values[i] = values[i-1];
      ord[i].depend = i-1;
      ord[i].excluded = true;
    }
  }
  
  return values;
}

bool *max_volumen(const intervalo_t *intervalos, uint n){
  bool *ret = new bool[n];
  for(uint x=0; x<n; x++)
    ret[x] = false;

  set *ord = orderer(intervalos, n);
  uint *val = weight(ord, n);
  uint j = n;

  while(j > 0){
    j--;
    if(j == 0 || (j > 0 && val[j] > val[j-1])){
      ret[ord[j].origin] = true;
      if(j == ord[j].depend)
        j = 0;
      else{
        j = ord[j].depend + 1;
        if(j == 0)
          ret[ord[j].origin] = true;
      }
    }else
      ret[ord[j].origin] = false;
  }

  delete[] val;
  delete[] ord;
  return ret;
}

