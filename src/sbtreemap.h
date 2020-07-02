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
    t = maintainLeft(t);
    return maintainRight(t);
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::MaintainLeft(SBTreeMap<KeyType, ValueType>::Tree t) {
    if (t == NULL || t->left == NULL) return NULL;
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
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Insert(SBTreeMap<KeyType, ValueType>::Tree t, KeyType key, Updater<KeyType, ValueType> *p_updater) {
    if (t == NULL) {
        ValueType value;
        if (p_updater->NeedNewKeyValuePair(&key, &value)) {
            t = new TNode(key, value);
        }
        return t;
    }
    if (key == t->key) {
        p_updater->UpdateKeyValuePair(&tree->key, &tree->value);
    }
    else if (key < t->key) {
        size_t tmp_size = GetSize(t->left);
        t->left = Insert(t->left, key, p_updater);
        if (GetSize(t->left) > tmp_size) {
            t->size++;
            t = MaintainLeft(t);
        }
    }
    else {
        size_t tmp_size = GetSize(t->right);
        t->right = Insert(t->right, key, p_updater);
        if (GetSize(t->right) > tmp_size) {
            t->size++;
            t = MaintainRight(t);
        }
    }
    return t;
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Search(SBTreeMap<KeyType, Value>::Tree t, KeyType key) {
    if (t == NULL) {
        return t;
    }
    if (key == t->key) {
        return t;
    }
    if (key < t->key) {
        return Search(t->left, key);
    }
    else {
        return Search(t->right, key);
    }
}

template<typename KeyType, typename ValueType>
typename SBTreeMap<KeyType, ValueType>::Tree SBTreeMap<KeyType, ValueType>::Remove(SBTreeMap<KeyType, ValueType>::Tree t, KeyType key) {
    if (t == NULL) {
        return t;
    }
    if (key == t->key) {
        if (t->left == NULL && t->right == NULL) {
            return NULL;
        }
        else if (t->left && t->right == NULL) {
            return t->left;
        }
        else if (t->left == NULL && t->right) {
            return t->right;
        }
        else {
            Tree left_max = t->left;
            while (left_max->right) 
                left_max = left_max->right;
            t->key = left_max->key;
            t->left = Remove(t->left, left_max->key);
            t->size--;
            t = MaintainRight(t);
            return t;
        }
    }
    else if (key < t->key) {
        size_t tmp_size = GetSize(t->left);
        t->left = Remove(t->left, key);
        if (GetSize(t->left) < tmp_size) {
            t->size--;
            t = MaintainRight(t);
        }
    }
    else {
        size_t tmp_size = GetSize(t->right);
        t->right = Remove(t->right, key);
        if (GetSize(t->right) < tmp_size) {
            t->size--;
            t = MaintainLeft(t);
        }
    }
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

template<typename KeyType, typename ValueType>
void SBTreeMap<KeyType, ValueType>::Update(KeyType key, Updater<KeyType, ValueType> *p_updater) {
    t_ = Insert(t_, key, p_updater);
}

template<typename KeyType, typename ValueType>
bool SBTreeMap<KeyType, ValueType>::Get(KeyType key, ValueType *p_value) {
    Tree t = NULL;
    t = Search(t_, key);
    if (t) {
        *p_value = t->value;
        return true;
    }
    return false;
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
