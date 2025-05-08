#include "heap.hpp"
#include <utility>
#include <vector>
int left(int i){
	return (i+1)*2-1;
}
int right(int i){
	return (i+1)*2;
}
void Heap::maxHeapfiy(std::vector<int>&vec,int index,int heapsize){
	int l=left(index);
	int r=right(index);
	int largest=index;
	if(l<=heapsize&&vec[l]>vec[largest]){
		largest=l;
	}
	if(r<=heapsize&&vec[r]>vec[largest]){
		largest=r;
	}
	if(largest!=index){
		std::swap(vec[index],vec[largest]);
		maxHeapfiy(vec,largest,heapsize);
	}
}

void Heap::init(std::vector<int>&vec){
	int len=vec.size();
	for(int i=len/2-1;i>=0;--i){
		maxHeapfiy(vec, i,len);
	}
}
