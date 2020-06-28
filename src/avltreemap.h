#pragma once
#include "treemap.h"

template<typename KeyType, typename ValueType>
class AVLTreeMap : public TreeMap<KeyType, ValueType> {
public:

	AVLTreeMap<KeyType, ValueType>() : tree(NULL), size_(0){}
	virtual ~AVLTreeMap<KeyType, ValueType>();

	virtual void Update(KeyType key, Updater<KeyType, ValueType> * p_updater);
	virtual bool Get(KeyType key, ValueType * p_value);
	virtual bool HasKey(KeyType key);
	virtual void Remove(KeyType key);
	virtual size_t Size();
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

	size_t GetHeight(Tree tree) { return tree ? tree->height : 0; }

	Tree LeftRotate(Tree tree);
	Tree RightRotate(Tree tree);

	Tree Search(Tree tree, KeyType key);
	Tree Insert(Tree tree, KeyType key, Updater<KeyType, ValueType> * p_updater);
	Tree Remove(Tree tree, KeyType key);

	void DeleteTree(Tree tree);

	size_t Max(size_t a, size_t b) {return a > b ? a : b; }
private:
	Tree tree;
	size_t size_;
};

/*------------------------------------------ AVL Tree --------------------------------------------*/

template<typename KeyType, typename ValueType>
typename  AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::LeftRotate(AVLTreeMap<KeyType, ValueType>::Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->height = this->Max(GetHeight(tree->left), GetHeight(tree->right)) + 1;
	k->height = this->Max(tree->height, GetHeight(k->right)) + 1;
	return k;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::RightRotate(AVLTreeMap<KeyType, ValueType>::Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->height = this->Max(GetHeight(tree->left), GetHeight(tree->right)) + 1;
	k->height = this->Max(GetHeight(k->left), tree->height) + 1;
	return k;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::Search(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key) {
	if (tree == NULL) {
		return NULL;
	}
	if (tree->key == key)
		return tree;
	else if (key < tree->key)
		return Search(tree->left, key);
	else
		return Search(tree->right, key);
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::Insert(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key, Updater<KeyType, ValueType> * p_updater) {
	if (tree == NULL) {
		ValueType value;
		if (p_updater->NeedNewKeyValuePair(&key, &value)) {
			size_++;
			return new TNode(key, value);
		}
		return NULL;
	}
	if (key == tree->key) {
		p_updater->UpdateKeyValuePair(&tree->key, &tree->value);
	}
	else if (key < tree->key) { //Insert into the left
		tree->left = Insert(tree->left, key, p_updater);
		if (GetHeight(tree->left) - GetHeight(tree->right) == 2) {
			if (key > tree->left->key) { //Insert into the right child of the left child
				tree->left = LeftRotate(tree->left);
			}
			tree = RightRotate(tree);
		}
	}
	else { //Insert into the right
		tree->right = Insert(tree -> right, key, p_updater);
		if (GetHeight(tree->right) - GetHeight(tree->left) == 2) {
			if (key < tree->right->key) { //Insert into the left child of the right child
				tree->right = RightRotate(tree->right);
			}
			tree = LeftRotate(tree);
		}
	}
	tree->height = this->Max(GetHeight(tree->left), GetHeight(tree->right)) + 1;
	return tree;
}

template<typename KeyType, typename ValueType>
typename AVLTreeMap<KeyType, ValueType>::Tree AVLTreeMap<KeyType, ValueType>::Remove(AVLTreeMap<KeyType, ValueType>::Tree tree, KeyType key) {
	if (tree == NULL) {
		return NULL; //Nothing removed
	}
	if (key < tree->key) { //The removing node (<key, value> pair) is in the left child
		tree->left = Remove(tree->left, key);
		//After removing a node form the left child of the tree, the height of the left tree may decrease 
		//and (height of the right subtree - height of the left subtree) may increase, similar to inserting 
		//an element into the right subtree.
		if (GetHeight(tree->right) - GetHeight(tree->left) == 2) {
			if (GetHeight(tree->right->left) > GetHeight(tree->right->right)) {
				tree->right = RightRotate(tree->right);
			}
			tree = LeftRotate(tree);
		}
	}
	else if (key > tree->key) {  //The removing node (<key, value> pair) is in the right child
		//Symmetric with removing nodes in the left subtree
		tree->right = Remove(tree->right, key);
		if (GetHeight(tree->left) - GetHeight(tree->right) == 2) {
			if (GetHeight(tree->left->right) > GetHeight(tree->left->left)) {
				tree->left = LeftRotate(tree->left);
			}
			tree = RightRotate(tree);
		}
	}
	else { //key == tree->key, tree is the node that needs to be removed
		if (tree->left && tree->right) { //tree has both left child and right child
			if (GetHeight(tree->left) > GetHeight(tree->right)) {
				//Replace tree by the node with maximum key in the left subtree
				//Find the node with maximum key in the left subtree
				Tree max = tree->left;
				while (max->right) {
					max = max->right;
				}
				tree->key = max->key; //replace
				tree->value = max->value; //replace
				tree->left = Remove(tree->left, max->key);
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
				tree->right = Remove(tree->right, min->key);
			}
		}
		else {
			Tree tmp = tree;
			tree = tree->left ? tree->left : tree->right;
			delete tmp; //delete
			size_--;
		}
	}
	if (tree) tree->height = this->Max(GetHeight(tree->left), GetHeight(tree->right)) + 1;
	return tree;
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::DeleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) DeleteTree(tree->left);
	if (tree->right) DeleteTree(tree->right);
	delete tree;
}

/*------------------------------------------ AVL Tree end --------------------------------------------*/

/*------------------------------------------- AVLTreeMap public functions -------------------------------------------*/

//Destructor
template<typename KeyType, typename ValueType>
AVLTreeMap<KeyType, ValueType>::~AVLTreeMap < KeyType, ValueType>() {
	DeleteTree(tree);
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::Update(KeyType key, Updater<KeyType, ValueType> * p_updater) {
	tree = Insert(tree, key, p_updater);
}

template<typename KeyType, typename ValueType>
bool AVLTreeMap<KeyType, ValueType>::Get(KeyType key, ValueType * p_value) {
	Tree result = Search(tree, key);
	if (result) {
		*p_value = result->value;
		return true;
	}
	return false;
}

template<typename KeyType, typename ValueType>
bool AVLTreeMap<KeyType, ValueType>::HasKey(KeyType key) {
	return (Search(tree, key) != NULL);
}

template<typename KeyType, typename ValueType>
void AVLTreeMap<KeyType, ValueType>::Remove(KeyType key) {
	tree = Remove(tree, key);
}

template<typename KeyType, typename ValueType>
size_t AVLTreeMap<KeyType, ValueType>::Size() {
	return size_;
}
