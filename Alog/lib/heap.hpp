#pragma once
#include <vector>
struct Node{
	int data;
	Node*left;
	Node*right;
};

class Heap{
public:
	void init(std::vector<int>&vec);
	void maxHeapfiy(std::vector<int>&vec,int i,int heapsize);
	void minHeapfiy(std::vector<int>&vec,int i,int heapsize);
private:
	Node* root_;
	bool isMax_;
};
