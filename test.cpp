/* ***********************************************************************
    > File Name: test.cpp
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Tue 06 Mar 2018 05:15:09 PM CST
*********************************************************************** */

#include "kmap.h"
#include <iostream>

using namespace std;

int main()
{
    kmap<int,int> test_map;
    test_map.insert(makePair(123,1));
    test_map.insert(makePair(52,2));
    test_map.insert(makePair(324,3));
    test_map.insert(makePair(436,4));
    test_map.insert(makePair(413,5));
    test_map.insert(makePair(525,6));
    test_map.insert(makePair(235,7));
    test_map.insert(makePair(63,8));
    test_map.insert(makePair(346,9));
    test_map.insert(makePair(521,10));
    test_map.insert(makePair(675,11));
    test_map.insert(makePair(240,12));

    test_map.erase(675);
    test_map.erase(413);
    
    kmap<int,int>::iterator it = test_map.begin();
    while(it != test_map.end()) {
        printf("%d %d\n",it->first,it->second);
        it++;
    }

    //puts("bfsPrint here:");
    //test_map.bfsPrint();
    return 0;
}

