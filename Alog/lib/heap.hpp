#pragma once
#include <vector>

class Heap {
public:
    void init(std::vector<int>& v);
    static void maxHeapfiy(std::vector<int>&vec,int index, int heapsize);

};

void heapSort(std::vector<int>& vec);


