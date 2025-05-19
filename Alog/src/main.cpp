#include "../lib/binarySearchTree.hpp"
#include "../lib/heap.hpp"
#include <vector>
#include<iostream>

void binary_test(){
    BinarySearchTree bst;
    bst.insert(15);
    bst.insert(6);
    bst.insert(18);
    bst.insert(3);
    bst.insert(7);

    std::cout << "Inorder: ";
    bst.inorderPrint();  // 3 6 7 15 18

    int key = 6;
    auto res = bst.search(key);
    if (res) {
        std::cout << "Found node with key = " << res->data_<< "\n";
    } else {
        std::cout << "Node not found\n";
    }
}
int main() {
  std::vector<int> vec{3, 1, 6, 8, 2, 5, 9, 4, 7};
  heapSort(vec);
  binary_test();
}
