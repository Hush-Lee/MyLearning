#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#include "../lib/heap.hpp"

std::vector<int> source{3,8,5,6,2,9,7,1,4};
std::vector<int> maxSort{1,2,3,4,5,6,7,8,9};
std::vector<int> minSort{9,8,7,6,5,4,3,2,1};

int main(int argc,char** argv){
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

TEST(sort,heapsort){
	heapSort(source);
	ASSERT_EQ(source,maxSort);
}
