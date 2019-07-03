#pragma once
#include "TreeMap.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

template<typename KeyType, typename ValueType>
class AVLTreeMap : public TreeMap<KeyType, ValueType> {
public:

	AVLTreeMap<KeyType, ValueType>();
	virtual ~AVLTreeMap<KeyType, ValueType>();

	virtual void put(KeyType key, ValueType value);
	virtual bool get(KeyType key, ValueType * p_value);
	virtual bool hasKey(KeyType key);
	virtual void remove(KeyType key);
	virtual size_t size();
private:

	//Define AVL Tree structure
	typedef struct TNode {
		KeyType key;
		ValueType value;
		struct TNode * left;
		struct TNode * right;
		size_t height;

		TNode(KeyType key, ValueType value)  : key(key), value(value), left(NULL), right(NULL), height(1) {}		
	} *Tree;

	size_t getHeight(Tree tree) { return tree ? tree->height : 0; }

	Tree leftRotate(Tree tree);
	Tree rightRotate(Tree tree);

	Tree search(Tree tree, KeyType key);
	Tree insert(Tree tree, KeyType key, ValueType value);
	Tree remove(Tree tree, KeyType key);

	void deleteTree(Tree tree);
private:
	Tree tree;
	size_t size_;
};

/*------------------------------------------ AVL Tree --------------------------------------------*/

template<typename KeyType, typename ValueType>
typename  AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::leftRotate(AVLTreeMap<KeyType, ValueType>::Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	k->height = max(tree->height, getHeight(k->right)) + 1;
	return k;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::rightRotate(AVLTreeMap<KeyType, ValueType>::Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	k->height = max(getHeight(k->left), tree->height) + 1;
	return k;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::search(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key) {
	if (tree == NULL) {
		return NULL;
	}
	if (tree->key == key)
		return tree;
	else if (key < tree->key)
		return search(tree->left, key);
	else
		return search(tree->right, key);
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::insert(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key, ValueType value) {
	if (tree == NULL) {
		size_++;
		return new TNode(key, value);
	}
	if (key == tree->key) {
		tree->value = value;		
	}
	else if (key < tree->key) { //Insert into the left
		tree->left = insert(tree->left, key, value);
		if (getHeight(tree->left) - getHeight(tree->right) == 2) {
			if (key > tree->left->key) { //Insert into the right child of the left child
				tree->left = leftRotate(tree->left);
			}
			tree = rightRotate(tree);
		}
	}
	else { //Insert into the right
		tree->right = insert(tree -> right, key, value);
		if (getHeight(tree->right) - getHeight(tree->left) == 2) {
			if (key < tree->right->key) { //Insert into the left child of the right child
				tree->right = rightRotate(tree->right);
			}
			tree = leftRotate(tree);
		}
	}
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	return tree;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::remove(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key) {
	if (tree == NULL) {
		return NULL; //Nothing removed
	}
	if (key < tree->key) { //The removing node (<key, value> pair) is in the left child
		tree->left = remove(tree->left, key);
		//After removing a node form the left child of the tree, the height of the left tree may decrease 
		//and (height of the right subtree - height of the left subtree) may increase, similar to inserting 
		//an element into the right subtree.
		if (getHeight(tree->right) - getHeight(tree->left) == 2) {
			if (getHeight(tree->right->left) > getHeight(tree->right->right)) {
				tree->right = rightRotate(tree->right);
			}
			tree = leftRotate(tree);
		}
	}
	else if (key > tree->key) {  //The removing node (<key, value> pair) is in the right child
		//Symmetric with removing nodes in the left subtree
		tree->right = remove(tree->right, key);
		if (getHeight(tree->left) - getHeight(tree->right) == 2) {
			if (getHeight(tree->left->right) > getHeight(tree->left->left)) {
				tree->left = leftRotate(tree->left);
			}
			tree = rightRotate(tree);
		}
	}
	else { //key == tree->key, tree is the node that needs to be removed
		if (tree->left && tree->right) { //tree has both left child and right child
			if (getHeight(tree->left) > getHeight(tree->right)) {
				//Replace tree by the node with maximum key in the left subtree
				//Find the node with maximum key in the left subtree
				Tree max = tree->left;
				while (max->right) {
					max = max->right;
				}
				tree->key = max->key; //replace
				tree->value = max->value; //replace
				tree->left = remove(tree->left, max->key);
			}
			else {
				//Replace tree by the node with minimum key in the right subtree
				//Find the node with minimum key in the right subtree
				Tree min = tree->right;
				while (min->left) {
					min = min->left;
				}
				tree->key = min->key; //replace
				tree->value = min->value; //replace
				tree->right = remove(tree->right, min->key);
			}
		}
		else {
			Tree tmp = tree;
			tree = tree->left ? tree->left : tree->right;
			delete tmp; //delete
			size_--;
		}
	}
	if (tree) tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	return tree;
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	delete tree;
}

/*------------------------------------------ AVL Tree end --------------------------------------------*/

/*------------------------------------------- AVLTreeMap public functions -------------------------------------------*/

//Constructor
template<typename KeyType, typename ValueType>
AVLTreeMap<KeyType, ValueType>::AVLTreeMap < KeyType, ValueType>()  : tree(NULL), size_(0) {
	
}

//Destructor
template<typename KeyType, typename ValueType>
AVLTreeMap<KeyType, ValueType>::~AVLTreeMap < KeyType, ValueType>() {
	deleteTree(tree);
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::put(KeyType key, ValueType value) {
	tree = insert(tree, key, value);
}

template<typename KeyType, typename ValueType>
bool AVLTreeMap<KeyType, ValueType>::get(KeyType key, ValueType * p_value) {
	Tree result = search(tree, key);
	if (result) {
		*p_value = result->value;
		return true;
	}
	return false;
}

template<typename KeyType, typename ValueType>
bool AVLTreeMap<KeyType, ValueType>::hasKey(KeyType key) {
	return (search(tree, key) != NULL);
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::remove(KeyType key) {
	tree = remove(tree, key);
}

template<typename KeyType, typename ValueType>
size_t AVLTreeMap<KeyType, ValueType>::size() {
	return size_;
}