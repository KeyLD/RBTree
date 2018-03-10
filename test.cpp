/* ***********************************************************************
    > File Name: test.cpp
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Tue 06 Mar 2018 05:15:09 PM CST
*********************************************************************** */

#include "rbtree.h"

using namespace std; 

int main()
{
    RBTree<int,int> rbtree;
    rbtree.insert(123,1);
    rbtree.insert(52,2);
    rbtree.insert(324,3);
    rbtree.insert(436,4);
    rbtree.insert(413,5);
    rbtree.insert(525,6);
    rbtree.insert(235,7);
    rbtree.insert(63,8);
    rbtree.insert(346,9);
    rbtree.insert(521,10);
    rbtree.insert(675,11);
    rbtree.insert(240,12);

    rbtree.erase(675);
    rbtree.erase(413);

    RBTree<int,int>::iterator it = rbtree.begin();
    while(it != rbtree.end()) {
        printf("%d %d\n",it->key,it->value);
        it = rbtree.nextPointer(it);
    }
    printf("%d %d\n",it->key,it->value);

    puts("bfsPrint here:");
    rbtree.bfsPrint();
    return 0;
}

