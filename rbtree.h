/* ***********************************************************************
    > File Name: rbtree.h
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Sat 27 Jan 2018 07:56:47 PM CST
*********************************************************************** */
#ifndef _RBTREE_H
#define _RBTREE_H

#define TEMPLATE template <typename K, typename V, class KeyOfValue, class Compare>

#include "pair.h"
#include "rbtree_iterator.h"
#include <cassert>
#include <queue>

TEMPLATE
class RBTree {
public:
    typedef K key_type;
    typedef V value_type;
    typedef V* pointer;
    typedef const V* const_pointer;
    typedef V& reference;
    typedef const V& const_reference;
    typedef RBTreeNode<V>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef RBTreeIterator<value_type, reference, pointer> iterator;
    typedef const RBTreeIterator<value_type, reference, pointer> const_iterator;

protected:
    typedef NodeBase* base_ptr;
    typedef RBTColor color_type;

    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftMost() const { return (link_type&)header->left; }
    link_type& rightMost() const { return (link_type&)header->right; }

    static link_type& left(link_type cNode) { return (link_type&)(cNode->left); }
    static link_type& right(link_type cNode) { return (link_type&)(cNode->right); }
    static link_type& parent(link_type cNode) { return (link_type&)(cNode->parent); }
    static reference value(link_type cNode) { return cNode->value; }
    static link_type& key(link_type cNode) { return KeyOfValue()(value(cNode)); }
    static color_type& color(link_type cNode) { return (color_type&)(cNode->color); }

    static link_type& left(base_ptr cNode) { return (link_type&)(cNode->left); }
    static link_type& right(base_ptr cNode) { return (link_type&)(cNode->right); }
    static link_type& parent(base_ptr cNode) { return (link_type&)(cNode->parent); }
    static reference value(base_ptr cNode) { return ((link_type)cNode)->value; }
    static link_type& key(base_ptr cNode) { return KeyOfValue()(value(link_type(cNode))); }
    static color_type& color(base_ptr cNode) { return (color_type&)(link_type(cNode)->color); }

    static link_type minimum(link_type cNode) { return (link_type)NodeBase::minimum(cNode); }
    static link_type maximum(link_type cNode) { return (link_type)NodeBase::maximum(cNode); }

private:
    bool insertFixUp(link_type);
    void eraseFixUp_left(link_type, link_type, link_type);
    void eraseFixUp_right(link_type, link_type, link_type);
    link_type leftRotate(link_type);
    link_type rightRotate(link_type); //尝试static 失败
    void moveNode(link_type, link_type);

    link_type findPrecursor(link_type);
    link_type findSuccessor(link_type);

    iterator __insert(base_ptr, base_ptr, const value_type&);
    bool insertNode(const link_type);

    void clear(link_type);
    void swap(color_type& ac,color_type& bc)
    {
        color_type tmp = ac;
        ac = bc;
        bc = tmp;
    }
    void swap(RBTree& s)
    {
        RBTree tmp = *this;
        *this = s;
        s = tmp;
    }

public:
    explicit RBTree(const Compare& compare = Compare())
        : node_count(0)
        , key_compare(compare)
    {
        header = new RBTreeNode<V>;
        root() = nullptr;
        leftMost() = header;
        rightMost() = header;
    }
    ~RBTree()
    {
        if (root() != nullptr)
            clear(root());
    }

    iterator find(K);
    pair<iterator, bool> insert(const value_type&); //修改返回值
    void erase(K);

    void bfsPrint();

    iterator begin() { return leftMost(); }
    iterator end() { return rightMost(); }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type count(const key_type& _key) { return find(_key) == end() ? 0 : 1; }
    void clear() { clear(root()); }
};

TEMPLATE
RBTreeNode<V>* RBTree<K, V, KeyOfValue, Compare>::leftRotate(link_type root)
{
    // 左旋必须确保 节点 与 节点的右孩子 不为空
    if (!root || !right(root)) {
        // error msg
        return nullptr;
    }

    link_type super_root;
    link_type right = this->right(root);
    link_type right_left = left(right);

    if (root == this->root()) {
        super_root = this->root();
        this->root() = this->right(root);
    } else
        super_root = parent(root);

    if (left(super_root) == root)
        left(super_root) = right;
    else if (this->right(super_root) == root)
        this->right(super_root) = right;
    parent(root) = super_root;

    left(right) = root;
    parent(root) = right;

    this->right(root) = right_left;
    if (right_left) //函数只保证旋转的两个节点不是nil
        parent(right_left) = root;

    return right;
}

TEMPLATE
RBTreeNode<V>* RBTree<K, V, KeyOfValue, Compare>::rightRotate(link_type root)
{
    // 右旋必须确保 节点 和 节点的左孩子 不为空
    if (!root || left(root)) {
        // error msg
        return nullptr;
    }

    link_type super_root;
    link_type left = this->left(root);
    link_type left_right = right(left);

    if (root == this->root()) {
        super_root = this->root();
        this->root() = this->left(root);
    } else
        super_root = parent(root);

    if (this->left(super_root) == root)
        this->left(super_root) = left;
    else if (right(super_root) == root)
        right(super_root) = left;
    parent(left) = super_root;

    right(left) = root;
    parent(root) = left;

    this->left(root) = left_right;
    if (left_right) // 函数只保证旋转的两个节点不是nil
        parent(left_right) = root;

    return left;
}

TEMPLATE
RBTreeIterator<V,V&,V*> RBTree<K, V, KeyOfValue, Compare>::find(K find_key)
{
    link_type y = header;
    link_type x = root();
    while (x != nullptr) {
        if(!key_compare(key(x),find_key)) { //compare不含等于号 若两值相等只能是两个compare都是false 妙！
            y = x,x=left(x);
        } else {
            x = right(x);
        }
    }
    iterator it = iterator(y);
    return (it == end()) or key_compare(find_key,key(it.node)) ? end() : it;
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::clear(link_type root)
{
    if (root->left)
        clear(left(root));
    if (root->right)
        clear(right(root));
    delete root;
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::moveNode(link_type replace, link_type deleted)
{
    link_type deleted_parent = parent(deleted);

    if (deleted_parent->left == deleted)
        deleted_parent->left = replace;
    else
        deleted_parent->right = replace;
    if (replace != nullptr)
        replace->parent = deleted_parent;
    delete deleted;
}

TEMPLATE
inline RBTreeNode<V>* RBTree<K, V, KeyOfValue, Compare>::findSuccessor(link_type root)
{
    link_type cNode = right(root);
    while (cNode->left != nullptr)
        cNode = left(cNode);
    return cNode;
}

TEMPLATE
inline RBTreeNode<V>* RBTree<K, V, KeyOfValue, Compare>::findPrecursor(link_type root)
{
    link_type cNode = left(root);
    while (cNode->right != nullptr)
        cNode = right(root);
    return cNode;
}

TEMPLATE
pair<RBTreeIterator<V, V&, V*>, bool> RBTree<K, V, KeyOfValue, Compare>::insert(const value_type& value)
{
    link_type parent = header;
    link_type cNode = root();
    key_type insert_key = KeyOfValue()(value);
    bool comp = true;
    while (cNode != nullptr) {
        parent = cNode;
        comp = key_compare(insert_key, key(cNode)); // insert_key < key(cNode) ?
        cNode = comp ? left(cNode) : right(cNode);  // 不是有环么 ?
    }
    iterator it = iterator(parent);
    if (comp) {
        if (it == begin())
            return pair<iterator, bool>(__insert(cNode, parent, value), true);
        else
            --it;
    }
    if (key_compare(key(it.node), insert_key))
        return pair<iterator, bool>(__insert(cNode, parent, value), true);
    return pair<iterator, bool>(it, false);
    //if (root() == nullptr) {
    //root() = new_node;
    //DYE_BLACK(root());
    //} else {
    //if (insertNode(new_node))
    //insertFixUp(new_node);
    //else
    //std::perror("insert error"); //修改
    //}
}

TEMPLATE
typename RBTree<K, V, KeyOfValue, Compare>::iterator
RBTree<K, V, KeyOfValue, Compare>::
    __insert(base_ptr _x, base_ptr _y, const value_type& value)
{
    auto cNode = (link_type)_x;
    auto y = (link_type)_y;
    link_type new_node;

    key_type insert_key = KeyOfValue()(value);

    if (y == header or cNode != nullptr or key_compare(insert_key, key(y))) {
        new_node = new RBTreeNode<value_type>;
        left(y) = new_node;

    }
}

TEMPLATE
bool RBTree<K, V, KeyOfValue, Compare>::insertNode(const link_type new_node)
{
    K insert_key = KeyOfValue()(new_node);
    link_type cNode = root();
    while (cNode != nullptr) {
        if (insert_key < key(cNode)) {
            if (cNode->left == nullptr) {
                cNode->left = new_node;
                new_node->parent = cNode;
                return true;
            } else
                cNode = left(cNode);
        } else if (insert_key > key(cNode)) {
            if (cNode->right == nullptr) {
                cNode->right = new_node;
                new_node->parent = cNode;
                return true;
            } else
                cNode = right(cNode);
        }
    }
    return false;
}

TEMPLATE
bool RBTree<K, V, KeyOfValue, Compare>::insertFixUp(link_type root)
{
    link_type parent = this->parent(root);
    while (parent != nullptr && IS_RED(parent)) { //只要父节点的颜色位红色，就保证有祖父节点，因为性质1
        link_type grandparent = this->parent(parent);
        if (grandparent->left == parent) {
            if (grandparent->right && IS_RED(grandparent->right)) {
                DYE_BLACK(parent);
                DYE_BLACK(grandparent->right);
                if (grandparent != this->root())
                    DYE_RED(grandparent);
                root = grandparent;
                parent = this->parent(root);
                continue;
            } else if (parent->right == root)
                leftRotate(parent);
            grandparent = rightRotate(grandparent);
            assert(grandparent != nullptr);
            DYE_BLACK(grandparent);
            DYE_RED(grandparent->left);
            DYE_RED(grandparent->right);
            break;
        } else {
            if (grandparent->left && IS_RED(grandparent->left)) {
                DYE_BLACK(parent);
                DYE_BLACK(grandparent->left);
                if (grandparent != this->root())
                    DYE_RED(grandparent);
                root = grandparent;
                parent = this->parent(root);
                continue;
            } else if (parent->left == root)
                rightRotate(parent);
            grandparent = leftRotate(grandparent);
            DYE_BLACK(grandparent);
            DYE_RED(grandparent->left);
            DYE_RED(grandparent->right);
            break;
        }
    }
    return true;
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::erase(K erase_key)
{
    auto deleted = (link_type)find(erase_key).node;
    if (deleted != nullptr) {
        if (deleted->right) {
            link_type replace = findSuccessor(deleted);

            value(deleted) = value(replace);
            key(deleted) = key(replace);

            link_type replace_right = right(replace), replace_parent = parent(replace);
            link_type replace_brother = left(replace_parent) == replace ? right(replace_parent) : left(replace_parent);
            bool is_replace_black = IS_BLACK(replace);
            moveNode(replace_right, replace);

            if (is_replace_black)
                eraseFixUp_right(replace_right, replace_parent, replace_brother);
        } else if (deleted->left) {
            link_type replace = findPrecursor(deleted);

            value(deleted) = value(replace);
            key(deleted) = key(replace);

            link_type replace_left = left(replace), replace_parent = parent(replace);
            link_type replace_brother = left(replace_parent) == replace ? right(replace_parent) : left(replace_parent);
            bool is_replace_black = IS_BLACK(replace);
            moveNode(replace_left, replace);

            if (is_replace_black)
                eraseFixUp_left(replace_left, replace_parent, replace_brother);
        } else {
            moveNode(right(deleted), deleted);
        }
    }
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::eraseFixUp_right(link_type deleted, link_type parent, link_type brother)
{
    if (IS_RED(deleted)) { //ok
        DYE_BLACK(deleted);
    } else if (IS_RED(brother)) {
        parent = leftRotate(parent);
        swap(parent->color, parent->left->color);
    } else if (IS_RED(brother->left)) { //ok
        link_type brother_left = left(brother);
        rightRotate(brother);
        COPY_COLOR(brother_left, parent);
        DYE_BLACK(parent);
        leftRotate(parent);
    } else if (IS_RED(brother->right)) { //ok
        COPY_COLOR(brother, parent);
        DYE_BLACK(parent);
        DYE_BLACK(brother->right);
        leftRotate(parent);
    } else if (IS_RED(parent)) { //ok
        DYE_RED(brother);
        DYE_BLACK(parent);
    } else { //ok
        DYE_RED(brother);
        deleted = parent;
        parent = this->parent(deleted);
        if (parent == nullptr)
            return;
        else if (parent->left == deleted)
            eraseFixUp_right(deleted, parent, right(parent));
        else
            eraseFixUp_left(deleted, parent, left(parent));
    }
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::eraseFixUp_left(link_type deleted, link_type parent, link_type brother)
{
    if (IS_RED(deleted)) { //ok
        DYE_BLACK(deleted);
    } else if (IS_RED(brother)) {
        parent = rightRotate(parent);
        swap(parent->color, parent->right->color);
    } else if (IS_RED(brother->right)) { //ok
        link_type brother_right = right(brother);
        leftRotate(brother);
        COPY_COLOR(brother_right, parent);
        DYE_BLACK(parent);
        rightRotate(parent);
    } else if (IS_RED(brother->left)) { //ok
        COPY_COLOR(brother, parent);
        DYE_BLACK(parent);
        DYE_BLACK(brother->left);
        rightRotate(parent);
    } else if (IS_RED(parent)) { //ok
        DYE_RED(brother);
        DYE_BLACK(parent);
    } else { //ok
        DYE_RED(brother);
        deleted = parent;
        parent = this->parent(deleted);
        if (parent == nullptr)
            return;
        else if (parent->left == deleted)
            eraseFixUp_right(deleted, parent, right(parent));
        else
            eraseFixUp_left(deleted, parent, left(parent));
    }
}

TEMPLATE
void RBTree<K, V, KeyOfValue, Compare>::bfsPrint()
{
    std::queue<link_type> que;
    que.push(root());
    while (!que.empty()) {
        RBTreeNode<int>* cNode = que.front();
        que.pop();
        //std::cout << cNode->key << "  " << (cNode->color == black ? "black" : "red") << std::endl;
        if (cNode->left != nullptr)
            que.push(left(cNode));
        if (cNode->right != nullptr)
            que.push(right(cNode));
    }
}

#endif
