cmake_minimum_required(VERSION 3.9)
project(RBTree)

set(CMAKE_CXX_STANDARD 11)

add_executable(RBTree
        kmap.h
        pair.h
        rbtree.h
        rbtree_iterator.h
        rbtree_node.h
        test.cpp)
