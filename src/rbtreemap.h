#pragma once
#include "treemap.h"

template<typename KeyType, typename ValueType>
class RBTreeMap : public TreeMap<KeyType, ValueType> {
public:
    virtual void Update(KeyType key, Updater<KeyType, ValueType> *p_updater);
	virtual bool Get(KeyType key, ValueType *p_value);
	virtual bool HasKey(KeyType key);
	virtual void Remove(KeyType key);
	virtual size_t Size();

    RBTreeMap<KeyType, ValueType>() : root_(NULL), size_(0) {}
    ~RBTreeMap<KeyType, ValueType>() {
        deleteTree(root_);
    }

private:
    
    enum Color {
        RED,
        BLACK
    };
    
    typedef struct TNode {
        KeyType key;
        ValueType value;
        Color color;
        struct TNode *parent, *left, *right;
        TNode(KeyType key, ValueType value) : key(key), value(value), color(RED), parent(NULL), left(NULL), right(NULL) {}
    } *Tree;
    Tree root_;
    size_t size_;

    void deleteTree(Tree t) {
        if (t == NULL)
            return;
        deleteTree(t->left);
        deleteTree(t->right);
        delete t;
    }
    
    Tree GetBrother(Tree t) {
        if (t == NULL)
            return NULL;
        Tree p = t->parent;
        if (p == NULL)
            return NULL;
        if (t->key < p->key)
            return p->right;
        else //t->key >= p->key
            return p->left;
    }

    void LeftRotate(Tree t);
    void RightRotate(Tree t);
    bool Insert(Tree t, KeyType key, Updater<KeyType, ValueType> *p_updater);
    void InsertFix(Tree t);
    Tree Search(Tree t, KeyType key);
    bool Remove(Tree t, KeyType key);
    void RemoveFix(Tree t, Tree parent);

};


/* ---------------------------------------------------------------- RBTreeMap private functions ---------------------------------------------------------------- */


template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::LeftRotate(RBTreeMap<KeyType, ValueType>::Tree t) {
    Tree p = t->parent;
    Tree k = t->right;
    t->right = k->left;
    if (k->left) k->left->parent = t;
    k->left = t;
    t->parent = k;
    if (p) {
        if (k->key < p->key) 
            p->left = k;
        else
            p->right = k;
        k->parent = p;
    }
    else {
        root_ = k;
        k->parent = NULL;
    }
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::RightRotate(RBTreeMap<KeyType, ValueType>::Tree t) {
    Tree p = t->parent;
    Tree k = t->left;
    t->left = k->right;
    if (k->right) k->right->parent = t;
    k->right = t;
    t->parent = k;
    if (p) {
        if (k->key < p->key)
            p->left = k;
        else
            p->right = k;
        k->parent = p;
    }
    else {
        root_ = k;
        k->parent = NULL;
    }
}

template<typename KeyType, typename ValueType>
bool RBTreeMap<KeyType, ValueType>::Insert(RBTreeMap<KeyType, ValueType>::Tree t, KeyType key, Updater<KeyType, ValueType> *p_updater) {
    Tree k = t, p = NULL;
    while (k) {
        if (key == k->key) {
            p_updater->UpdateKeyValuePair(&k->key, &k->value);
            return false;
        }
        if (key < k->key) {
            p = k;
            k = k->left;
        }
        else { //key > k->key
            p = k;
            k = k->right;
        }
    }
    ValueType value;
    if (!p_updater->NeedNewKeyValuePair(&key, &value))
        return false;
    k = new TNode(key, value);
    if (p == NULL) {
        root_ = k;
    }
    else {
        if (key < p->key)
            p->left = k;
        else
            p->right = k;
        k->parent = p;
    }
    InsertFix(k);
    return true;
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::InsertFix(RBTreeMap<KeyType, ValueType>::Tree t) {
    while (1) {
        Tree p = t->parent;
        if (p == NULL) {
            t->color = BLACK;
            break;
        }
        if (p->color == BLACK)
            break;
        Tree u = GetBrother(p);
        Tree gp = p->parent;
        if (u && u->color == RED) {
            p->color = BLACK;
            u->color = BLACK;
            gp->color = RED;
            t = gp;
            continue;
        }
        else { //u == NULL || u->color == BLACK
            if (p == gp->left) {
                if (t == p->right) {
                    LeftRotate(p);
                    t = p;
                    continue;
                }
                else { //t == p->left
                    p->color = BLACK;
                    gp->color = RED;
                    RightRotate(gp);
                    break;
                }
            }
            else { //p == gp->right
                if (t == p->left) {
                    RightRotate(p);
                    t = p;
                    continue;
                }
                else { //t == p->right
                    p->color = BLACK;
                    gp->color = RED;
                    LeftRotate(gp);
                }
            }
        }
    }
}

template<typename KeyType, typename ValueType>
typename RBTreeMap<KeyType, ValueType>::Tree RBTreeMap<KeyType, ValueType>::Search(RBTreeMap<KeyType, ValueType>::Tree t, KeyType key) {
    while (t) {
        if (key == t->key)
            break;
        else if (key < t->key)
            t = t->left;
        else //key > t->key
            t = t->right;
    }
    return t;
}

template<typename KeyType, typename ValueType>
bool RBTreeMap<KeyType, ValueType>::Remove(RBTreeMap<KeyType, ValueType>::Tree t, KeyType key) {
    while (t) {
        if (key < t->key)
            t = t->left;
        else if (key > t->key)
            t = t->right;
        else { //key == t->key
			Tree original = NULL;
            if (t->left && t->right) {
                Tree left_max = t->left;
                while (left_max->right) {
                    left_max = left_max->right;
                }
				original = t;
                t = left_max;
            }
            Tree parent = t->parent;
            bool flag = parent && t->key < parent->key;
            Color t_color = t->color;
            Tree child = t->left ? t->left : t->right;
			if (original != NULL) {
				original->key = t->key;
				original->value = t->value;
			}
            delete t;
            if (child)
                child->parent = parent;
            if (parent) {
                if (flag)
                    parent->left = child;
                else
                    parent->right = child;
            }
            else
                root_ = child;
            if (t_color == BLACK)
                RemoveFix(child, parent);
            return true;
        }        
    }
    return false;
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::RemoveFix(RBTreeMap<KeyType, ValueType>::Tree t, RBTreeMap<KeyType, ValueType>::Tree parent) {
    while (1) {
        if (t && t->color == RED) {
            t->color = BLACK;
            break;
        }
        //t == NULL || t->color == BLACK
        if (parent == NULL) {
            break;
        }        
        Tree br;
        if (parent->left == t)
            br = parent->right;
        else //parent->right == t
            br = parent->left;
        if (br->color == RED) {
            br->color = BLACK;
            parent->color = RED;
            if (t == parent->left)
                LeftRotate(parent);
            else //t == parent->right;
                RightRotate(parent);
            continue;
        }
        else {
            if (!(br->left && br->left->color == RED) && !(br->right && br->right->color == RED)) {
                br->color = RED;
                t = parent;
                parent = t->parent;
                continue;
            }
            else { // (br->left && br->left->color == RED) || (br->right && br->right->color == RED)
                if (t == parent->left) {
                    if (!(br->right && br->right->color == RED)) {
                        br->left->color = BLACK;
                        br->color = RED;
                        RightRotate(br);
                        continue;
                    }
                    else { //br->right && br->right->color == RED
                        Color tmp = br->color;
                        br->color = parent->color;
                        parent->color = tmp;
                        LeftRotate(parent);
                        br->right->color = BLACK;
                        break;
                    }
                }
                else { //t == parent->right
                    if (!(br->left && br->left->color == RED)) {
                        br->right->color = BLACK;
                        br->color = RED;
                        LeftRotate(br);
                        continue;
                    }
                    else { //br->left && b->left->color == RED
                        Color tmp = parent->color;
                        parent->color = br->color;
                        br->color = tmp;
                        RightRotate(parent);
                        br->left->color = BLACK;
                        break;
                    }
                }
            }
        }
    }
}


/* ---------------------------------------------------------------- RBTreeMap public functions ---------------------------------------------------------------- */

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::Update(KeyType key, Updater<KeyType, ValueType> *p_updater) {
    if (Insert(root_, key, p_updater)) size_++;
}

template<typename KeyType, typename ValueType>
bool RBTreeMap<KeyType, ValueType>::Get(KeyType key, ValueType *p_value) {
    Tree t = Search(root_, key);
    if (t) {
        *p_value = t->value;
        return true;
    }
    return false;
}

template<typename KeyType, typename ValueType>
bool RBTreeMap<KeyType, ValueType>::HasKey(KeyType key) {
    return Search(root_, key) != NULL;
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::Remove(KeyType key) {
    if (Remove(root_, key)) size_--;
}

template<typename KeyType, typename ValueType>
size_t RBTreeMap<KeyType, ValueType>::Size() {
    return size_;
}
