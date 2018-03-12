/* ***********************************************************************
    > File Name: kmap.h
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Mon 12 Mar 2018 12:06:31 PM CST
*********************************************************************** */
#ifndef _KMAP_H
#define _KMAP_H

#include "pair.h"
#include "rbtree.h"

#define KMAP_TEMPLATE template <typename K, typename T, class Compare = std::less<K>>

KMAP_TEMPLATE
class kmap {
public:
    typedef K key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<K, T> value_type; // const ?
    typedef Compare key_compare;

private:
    typedef RBTree<key_type, value_type, std::_Select1st<value_type>, key_compare> rep_type;
    rep_type tree;

public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // 构造
    kmap() {}
    explicit kmap(const Compare& compare)
        : tree(compare)
    {
    }
    kmap(const kmap<K, T, Compare>& _kmap)
        : kmap(_kmap.tree)
    {
    }
    kmap<K, T, Compare>& operator=(const kmap<K, T, Compare>& _kmap)
    {
        tree = _kmap.tree;
        return *this;
    }

    //接口
    iterator begin() { return tree.begin(); }
    //const_iterator begin() const { return tree.begin(); }
    iterator end() { return tree.end(); }
    //const_iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }

    T& operator[](const key_type& _key) { return (*((insert(value_type(_key, T()))).first)).second; }
    void swap(kmap<K, T, Compare>& cTree) { tree.swap(cTree.tree); }

    //核心操作
    pair<iterator,bool> insert(const value_type& v) { return tree.insert(v); }
    void erase(iterator pos) { tree.erase(pos); }
    size_type erase(const key_type& _key) { tree.erase(_key); }
    void clear() { tree.clear(); }

    //map操作
    iterator find(const key_type& _key) { return tree.find(_key); }
    // iterator find(const key_type& _key) const { return tree.find(_key); }
    size_type count(const key_type& _key) { return tree.count(_key); }

    template <typename _K, typename _T, class _Compare>
    friend bool operator==(const kmap<_K, _T, _Compare>&, const kmap<_K, _T, _Compare>&);
    template <typename _K, typename _T, class _Compare>
    friend bool operator<(const kmap<_K, _T, _Compare>&, const kmap<_K, _T, _Compare>&);
};

KMAP_TEMPLATE
inline bool operator==(const kmap<K, T, Compare>& x,
    const kmap<K, T, Compare>& y)
{
    return x.tree == y.tree;
}

KMAP_TEMPLATE
inline bool operator<(const kmap<K, T, Compare>& x,
    const kmap<K, T, Compare>& y)
{
    return x.tree < y.tree;
}

#endif
