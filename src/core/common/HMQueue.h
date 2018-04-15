#pragma once

#include <iostream>
#include <vector>
using namespace std;

template<class T>
class HMQueue : public vector < T >
{
public:
    void push(T t)
    {
        this->push_back(t);
    }

    void pop()
    {
        this->erase(this->begin());
    }

    //增加一个元索 
    void add(T t)
    {
        this->push(t);
    }

    //移除并返问队列头部的元素    如果队列为空，则返回null
    T poll()
    {
        if (this->size() <= 0)
        {
            return NULL;
        }

        T t = this->front();
        this->pop();

        return t;
    }
    //返回队列头部的元素             如果队列为空，则返回null
    T peek()
    {
        if (this->size() <= 0)
        {
            return NULL;
        }

        T t = this->front();

        return t;
    }

    void removeLast()
    {
        T a = this->back();
        delete a;
        this->pop_back();
    }
};