#include "RbTree.hpp"
template<class K,class E>
BsTree<K,E>::BsTree():left(nullptr),right(nullptr){
}

template <class K,class E>
BsTree<K,  E>* const BsTree<K, E>::min(){
	BsTree<K,  E>* result;
	result=this;
	while(result->left==nullptr){
		result=result->left;
	}
	return result;
}

template <class K,class E>
void BsTree<K,E>::insert(BsTree<K,E> node){
	
	if(this->key_>node->getKey()){
		this->left->insert(node);
	}else{
		this->right->insert(node);
	}
	
}
