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
    void Insert(Tree t, KeyType key, Update<KeyType, ValueType> *p_updater);
    void InsertFix(Tree t);
    Tree Search(Tree t, KeyType key);

};


/* ---------------------------------------------------------------- RBTreeMap private functions ---------------------------------------------------------------- */


template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::LeftRotate(RBTreeMap<KeyType, ValueType>::Tree t) {
    Tree p = t->parent;
    Tree k = t->right;
    t->right = k->left;
    k->left->parent = t;
    k->left = t;
    t->parent = k;
    if (p) {
        if (k->key < p-<key) 
            p->left = k;
        else
            p->right = k;
        k->parent = p;
    }
    else
        root_ = k;
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::RightRotate(RBTreeMap<KeyType, ValueType>::Tree t) {
    Tree p = t->parent;
    Tree k = t->left;
    t->left = k->right;
    k->right->parent = t;
    k->right = t;
    t->parent = k;
    if (p) {
        if (k->key < p->key)
            p->left = k;
        else
            p->right = k;
        k->parent = p;
    }
    else
        root_ = k;
}

template<typename KeyType, typename ValueType>
void RBTreeMap<KeyType, ValueType>::Insert(RBTreeMap<KeyType, ValueType>::Tree t, KeyType key, Updater<KeyType, ValueType> *p_updater>) {    
    Tree k = t, p = NULL;
    while (1) {
        if (k == NULL)
            break;
        if (key == k->key) {
            p_updater->UpdateKeyValuePair(&k->key, &k->value);
            return;
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
        return;
    k = new TNode(key, value);
    if (p == NULL) {
        k->color = BLACK;
        root_ = k;
    }
    else {
        if (key < p->key)
            p->left = k;
        else
            p->right = k;
        k->parent = p;
        InsertFix(t, k);
    }
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
        if (u == NULL) {
            p->color = BLACK;
            gp->color = RED;
            RightRotate(gp);
            break;
        }
        if (u->color == RED) {
            p->color = BLACK;
            u->color = BLACK;
            gp->color = RED;
            t = gp;
            continue;
        }
        if (u->color == BLACK) {
            p->color = BLACK;
            gp->color = RED;
            LeftRotate(gp);
            break;
        }
    }
}
