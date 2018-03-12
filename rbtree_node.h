/* ***********************************************************************
    > File Name: rbtree_node.h
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Mon 12 Mar 2018 09:47:00 AM CST
*********************************************************************** */
#ifndef _RBTREE_NODE_H
#define _RBTREE_NODE_H

#define DYE_RED(node) (node)->color = red
#define DYE_BLACK(node) (node)->color = black
#define COPY_COLOR(copy, copied) (copy)->color = (copied)->color
#define IS_RED(node) ((node) != nullptr and (node)->color == red)
#define IS_BLACK(node) ((node) == nullptr or (node)->color == black)

enum RBTColor {
    red,
    black
};

class NodeBase {
public:
    typedef NodeBase* link_type;

    RBTColor color;
    link_type parent;
    link_type left, right;

    static link_type minimum(link_type cNode)
    {
        if (cNode == nullptr)
            return nullptr;
        while (cNode->left)
            cNode = cNode->left;
        return cNode;
    }

    static link_type maximum(link_type cNode)
    {
        if (cNode == nullptr)
            return nullptr;
        while (cNode->right)
            cNode = cNode->right;
        return cNode;
    }
};

template <typename V>
class RBTreeNode : public NodeBase {
public:
    typedef RBTreeNode* link_type;

    V value;

    RBTreeNode(V _value = 0)
        : value(_value)
    {
        color = red;
        left = right = parent = nullptr;
    }
    ~RBTreeNode() {}
};

#endif
