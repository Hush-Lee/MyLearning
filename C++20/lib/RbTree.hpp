#pragma once
template <class K,class E>
class BsTree{
public:
	BsTree();
	~BsTree();
	BsTree<K,E>* const find(K key);
	void insert(BsTree<K,E> node);
	void remove(K key);
	BsTree<K,E>* const max();
	BsTree<K,E>* const min();
	K getKey(){return key_;}
	E getValue(){return value_;}
private:
	BsTree<K,E>*left;
	BsTree<K,E>*right;
	K key_;
	E value_;
};
