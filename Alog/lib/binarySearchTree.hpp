#pragma once
struct TreeNode {
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
  int data_;
  TreeNode(int data)
      : data_(data), left(nullptr), right(nullptr), parent(nullptr) {}
};

class BinarySearchTree {
public:
  BinarySearchTree();
  ~BinarySearchTree();
  void insert(int key);
  TreeNode *search(int key) const;
  void inorderPrint() const;
  void remove(int key);
  void transplant(TreeNode* source,TreeNode* target);
  TreeNode* minimun(TreeNode*node);
private:
  TreeNode *root;
  void inorderPrint(TreeNode *node) const;
  void destroy(TreeNode *node);
};
