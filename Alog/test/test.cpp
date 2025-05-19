#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

#include "../lib/binarySearchTree.hpp"
#include "../lib/heap.hpp"

std::vector<int> source{3, 8, 5, 6, 2, 9, 7, 1, 4};
std::vector<int> maxSort{1, 2, 3, 4, 5, 6, 7, 8, 9};
std::vector<int> minSort{9, 8, 7, 6, 5, 4, 3, 2, 1};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(sort, heapsort) {
  heapSort(source);
  ASSERT_EQ(source, maxSort);
}

TEST(BinarySearchTreeTest, InsertAndSearch) {
  BinarySearchTree bst;

  bst.insert(15);
  bst.insert(6);
  bst.insert(18);
  bst.insert(3);
  bst.insert(7);

  // 验证搜索存在的节点
  TreeNode *node = bst.search(6);
  ASSERT_NE(node, nullptr);
  EXPECT_EQ(node->data_, 6);

  node = bst.search(18);
  ASSERT_NE(node, nullptr);
  EXPECT_EQ(node->data_, 18);
  
  bst.remove(18);
  node=bst.search(18);
  EXPECT_EQ(node,nullptr);

  // 验证搜索不存在的节点
  node = bst.search(10);
  EXPECT_EQ(node, nullptr);
}
