#include <iostream>
#include <deque>
#include "url.hpp"
#include "binHeap.hpp"

using namespace std;

binHeap::binHeap(int max){
  maxSize = max;
}

binHeap::heapify(){
  
}

void binHeap::insert(url a){
  if (queue.size() == maxSize){
    url last = queue.back();
    if (a.getSize() < last.getSize()){
      queue.pop_back();
      queue.push_front(a);
    }
  }
  heapify();
}



/*
void heapify (int tam, int** array, int pos) //leva um elemento para sua posição devida no heap
{
	int esq = left(pos);
	int dir = right(pos);
	int lar;
	if ((esq < tam) && (array[esq][0] < array[pos][0]))	lar = esq;
	else	lar = pos;

	if ((dir < tam) && (array[dir][0] < array[lar][0]))	lar = dir;

	if  (lar != pos)
	{
		int* aux = array[pos];
		array[pos] = array[lar];
		array[lar] = aux;
		heapify(tam, array, lar);
	}
}
