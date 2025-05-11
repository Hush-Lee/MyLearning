#include "heap.hpp"
#include <algorithm>
#include <cassert>
#include <vector>
int left(int i){        return i*2+1;}
int right(int i){       return i*2+2;}

void Heap::maxHeapfiy(std::vector<int>&vec,int index,int heapsize){
        int l=left(index);
        int r=right(index);
        int largest=index;
        if(l<heapsize&&vec[l]>vec[largest]){
                largest=l;
        }
        if(r<heapsize&&vec[r]>vec[largest]){
                largest=r;
        }
        if(largest!=index){
                std::swap(vec[index], vec[largest]);
                maxHeapfiy(vec,largest,heapsize);
        }
}

void Heap::init(std::vector<int>&vec){
        int len=vec.size();
        for(int i=len/2-1;i>=0;--i){
                maxHeapfiy(vec,i,len);
        }
}

void heapSort(std::vector<int>& vec){
        Heap heap;
        heap.init(vec);
        int heapsize=vec.size();
        for(int i=vec.size();i>0;--i){
                std::swap(vec[0],vec[i-1]);
                --heapsize;
                heap.maxHeapfiy(vec,0,heapsize);
        }
}


