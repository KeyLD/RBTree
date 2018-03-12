/* ***********************************************************************
    > File Name: rbtree_iterator.h
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Mon 12 Mar 2018 09:48:11 AM CST
*********************************************************************** */
#ifndef _RBTREE_ITERATOR_H
#define _RBTREE_ITERATOR_H

#include "rbtree_node.h"

class RBTreeIteratorBase {
public:
    typedef NodeBase::link_type link_type;
    link_type node;

    void increment()
    {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        } else {
            link_type parent = node->parent;
            while (node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            if (node->right != parent)
                node = parent;
        }
    }
    void decrement()
    {
        if (IS_RED(node) and node->parent->parent == node)
            node = node->right;
        else if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr)
                node = node->right;
        } else {
            link_type parent = node->parent;
            while (node == parent->left) {
                node = parent;
                parent = parent->parent;
            }
            node = parent;
        }
    }
};

template <typename V, typename R, typename P>
class RBTreeIterator : public RBTreeIteratorBase {
public:
    typedef V value_type;
    typedef R reference;
    typedef P pointer;
    typedef RBTreeIterator<V, V&, V*> iterator;
    typedef RBTreeIterator<V, const V&, const V*> const_iterator;
    typedef RBTreeIterator<V, R, P> self;
    typedef RBTreeNode<V>* link_type;

    RBTreeIterator() {}
    RBTreeIterator(link_type _node) { node = _node; }
    RBTreeIterator(const iterator& it) { node = it.node; }

    reference operator*() const { return link_type(node)->value; }
    pointer operator->() const { return &(operator*()); }

    self& operator++()
    {
        increment();
        return *this;
    }

    self operator++(int)
    {
        iterator tmp = *this;
        increment();
        return tmp;
    }

    self& operator--()
    {
        decrement();
        return *this;
    }

    self operator--(int)
    {
        iterator tmp = *this;
        decrement();
        return tmp;
    }

    bool operator==(const iterator& it) const { return node == it.node; }
    bool operator!=(const iterator& it) const { return node != it.node; }
};

#endif
