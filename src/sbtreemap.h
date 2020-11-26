#pragma once
#include "treemap.h"

template<typename KeyType, typename ValueType>
class SBTreeMap : public TreeMap<KeyType, ValueType> {
public:
    virtual void Update(KeyType key, Updater<KeyType, ValueType> *p_updater);
    virtual bool Get(KeyType key, ValueType *p_value);
    virtual bool HasKey(KeyType key);
    virtual void Remove(KeyType key);
    virtual size_t Size();

    SBTreeMap<KeyType, ValueType>() : t_(NULL) {
        
    }

    ~SBTreeMap<KeyType, ValueType>() {
        DeleteTree(t_);
    }
private:
    typedef struct TNode {
        KeyType key;
        ValueType value;
        struct TNode *left, *right;
        size_t size;
        TNode(KeyType key, ValueType value) : key(key), value(value), left(NULL), right(NULL), size(1) {}
    } *Tree;

    size_t GetSize(Tree t) { return t ? t->size : 0; }

    Tree LeftRotate(Tree t);
    Tree RightRotate(Tree t);
    Tree Maintain(Tree t);
    Tree MaintainLeft(Tree t);
    Tree MaintainRight(Tree t);

    Tree Insert(Tree t, KeyType key, Updater<KeyType, ValueType> *p_updater);
    Tree Search(Tree t, KeyType key);
    Tree Remove(Tree t, KeyType key);

    void DeleteTree(Tree t);
private:
    Tree t_;
};


/* ---------------------------------------------------------------- SBTreeMap private functions ---------------------------------------------------------------- */

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::LeftRotate(SBTreeMap<KeyType, ValueType>::Tree t) {
    Tree k = t->right;
    t->right = k->left;
    k->left = t;
    k->size = t->size;
    t->size = GetSize(t->left) + GetSize(t->right) + 1;
    return k;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::RightRotate(SBTreeMap<KeyType, ValueType>::Tree t) {
    Tree k = t->left;
    t->left = k->right;
    k->right = t;
    k->size = t->size;
    t->size = GetSize(t->left) + GetSize(t->right) + 1;	
    return k;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Maintain(SBTreeMap<KeyType, ValueType>::Tree t) {
    return MaintainRight(
        MaintainLeft(t)
       );
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::MaintainLeft(SBTreeMap<KeyType, ValueType>::Tree t) {
    if (t == NULL || t->left == NULL) return t;
    if (t->left->left && t->left->left->size > GetSize(t->right)) {
        t = RightRotate(t);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    if (t->left->right && t->left->right->size > GetSize(t->right)) {
        t->left = LeftRotate(t->left);
        t = RightRotate(t);
        t->left = Maintain(t->left);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    return t;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::MaintainRight(SBTreeMap<KeyType, ValueType>::Tree t) {
    if (t == NULL || t->right == NULL) return t;
    if (t->right->right && t->right->right->size > GetSize(t->left)) {
        t = LeftRotate(t);
        t->left = Maintain(t->left);
        t = Maintain(t);
    }
    if (t->right->left && t->right->left->size > GetSize(t->left)) {
        t->right = RightRotate(t->right);
        t = LeftRotate(t);
        t->left = Maintain(t->left);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    return t;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Insert(SBTreeMap<KeyType, ValueType>::Tree t, KeyType key, Updater<KeyType, ValueType> *p_Updater) {
    if (t == NULL) {
        ValueType value;
        if (p_Updater->NeedNewKeyValuePair(&key, &value)) {
            t = new TNode(key, value);
        }
        return t;
    }
    if (key == t->key) {
        p_Updater->UpdateKeyValuePair(&t->key, &t->value);
    }
    else if (key < t->key) {
        size_t left_size = GetSize(t->left);
        t->left = Insert(t->left, key, p_Updater);
        if (GetSize(t->left) > left_size) {
            t->size++;
            t = MaintainLeft(t);
        }
    }
    else { //key > t->key
        size_t right_size = GetSize(t->right);
        t->right = Insert(t->right, key, p_Updater);
        if (GetSize(t->right) > right_size) {
            t->size++;
            t = MaintainRight(t);
        }
    }
    return t;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Search(SBTreeMap<KeyType, ValueType>::Tree t, KeyType key) {
    if (t == NULL) return t;
    if (key == t->key) 
        return t;	
    else if (key < t->key) 
        return Search(t->left, key);
    else //key > t->key
        return Search(t->right, key);
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Remove(SBTreeMap<KeyType, ValueType>::Tree t, KeyType key) {
    if (t == NULL)
        return t;
    if (key < t->key) {
        size_t left_size = GetSize(t->left);
        t->left = Remove(t->left, key);
        if (GetSize(t->left) < left_size) {
            t->size--;
            t = MaintainRight(t);
        }
    }
    else if (key > t->key) {
        size_t right_size = GetSize(t->right);
        t->right = Remove(t->right, key);
        if (GetSize(t->right) < right_size) {
            t->size--;
            t = MaintainLeft(t);
        }
    }
    else { //key == t->key
        if (t->left == NULL || t->right == NULL) {
            Tree tmp = t;
            if (t->left) t = t->left;
            else if (t->right) t = t->right;
            else t = NULL;
            delete tmp;
        }
        else {
            Tree leftMax = t->left;
            while (leftMax->right)
                leftMax = leftMax->right;
            t->key = leftMax->key;
            t->value = leftMax->value;
            t->left = Remove(t->left, leftMax->key);
            t->size--;
            t = MaintainRight(t);
        }
    }
    return t;
}

template<typename KeyType, typename ValueType>
void SBTreeMap<KeyType, ValueType>::DeleteTree(SBTreeMap<KeyType, ValueType>::Tree t) {
    if (t == NULL)
        return;
    DeleteTree(t->left);
    DeleteTree(t->right);
    delete t;
}
/* ---------------------------------------------------------------- SBTreMap public functions ---------------------------------------------------------------- */

template <typename KeyType, typename ValueType>
void SBTreeMap<KeyType, ValueType>::Update(KeyType key, Updater<KeyType, ValueType> *p_updater) {
    t_ = Insert(t_, key, p_updater);
}

template<typename KeyType, typename ValueType>
bool SBTreeMap<KeyType, ValueType>::Get(KeyType key, ValueType *p_value) {
    Tree t = Search(t_, key);
    if (t == NULL)
        return false;
    *p_value = t->value;
    return true;
}

template<typename KeyType, typename ValueType>
bool SBTreeMap<KeyType, ValueType>::HasKey(KeyType key) {
    return Search(t_, key) != NULL;
}

template<typename KeyType, typename ValueType>
void SBTreeMap<KeyType, ValueType>::Remove(KeyType key) {
    t_ = Remove(t_, key);
}

template<typename KeyType, typename ValueType>
size_t SBTreeMap<KeyType, ValueType>::Size() {
    return GetSize(t_);
}
