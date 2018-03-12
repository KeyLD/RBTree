/* ***********************************************************************
    > File Name: pair.h
    > Author: Key
    > Mail: keyld777@gmail.com
    > Created Time: Mon 12 Mar 2018 01:25:58 PM CST
*********************************************************************** */
#ifndef _PAIR_H
#define _PAIR_H

template <typename T1, typename T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;
    pair()
        : first(T1())
        , second(T2())
    {
    }
    pair(const T1& a, const T2& b)
        : first(a)
        , second(b)
    {
    }

    pair& operator=(const pair& p)
    {
        this->first = p.first;
        this->second = p.second;
        return *this;
    }
    bool operator==(const pair& p) const { return this->first == p.first and this->second == p.second; }
};

template <typename T1, typename T2>
pair<T1,T2> makePair(T1 a, T2 b)
{
    return pair<T1,T2>(a,b);
};

template <typename Pair>
class select1st {
public:
    typename Pair::first_type& operator()(Pair& x) { return  x.first; }
};

#endif
