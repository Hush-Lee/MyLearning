#include "binarySearchTree.hpp"
#include <iostream>
#include <ostream>
void BinarySearchTree::insert(int key) {
  TreeNode *newNode = new TreeNode(key);
  TreeNode *p = nullptr;
  TreeNode *now = root;
  while (now != nullptr) {
    p = now;
    if (newNode->data_ < now->data_) {
      now = now->left;
    } else {
      now = now->right;
    }
  }
  newNode->parent = p;
  if (p == nullptr) {
      root=newNode;
  }else if(p->data_>newNode->data_){
      p->left=newNode;
  }else{
      p->right=newNode;
  }
}

TreeNode* BinarySearchTree::search(int key)const{
    TreeNode* now=root;
    while(now&&now->data_!=key){
        if(now->data_<key){
            now=now->right;
        }else{
            now=now->left;
        }
    }
    return now;
}


void BinarySearchTree::inorderPrint()const{
    inorderPrint(root);
    std::cout<<std::endl;
}

void BinarySearchTree::inorderPrint(TreeNode* node)const{
    if(node!=nullptr){
        inorderPrint(node->left);
        std::cout<<node->data_<<" ";
        inorderPrint(node->right);
    }
}

BinarySearchTree::BinarySearchTree():root(nullptr){}

BinarySearchTree::~BinarySearchTree(){
    destroy(root);
}
void BinarySearchTree::destroy(TreeNode* node){
    if(node!=nullptr){
        destroy(node->left);
        destroy(node->right);
        delete node;
        node =nullptr;
    }
}


TreeNode* BinarySearchTree::minimun(TreeNode*node){
    while(node->left){
        node=node->left;
    }
    return node;
}
void BinarySearchTree::transplant(TreeNode* source,TreeNode* target){
    if(source->parent==nullptr){
        root=target;
    }else if(source==source->parent->left){
        source->parent->left=target;
    }else{
        source->parent->right=target;
    }
    if(target!=nullptr){
        target->parent=source->parent;
    }
}

void BinarySearchTree::remove(int key){
    TreeNode*target=search(key);
    if(target==nullptr){
        return;
    }
    if(target->left==nullptr){
        transplant(target,target->right);
    }else if(target->right==nullptr){
        transplant(target,target->left);
    }else{
        auto replace=minimun(target->right);
        if(replace->parent!=target){
            transplant(replace,replace->right);
            replace->right=target->right;
            replace->right->parent=replace;
        }
        transplant(target,replace);
        replace->left=target->left;
        replace->left->parent=replace;
    }
    delete target;
}
