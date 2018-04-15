#pragma once

#include <iostream>
#include <vector>
using namespace std;

template<class T>
class HMStack: public vector < T >
{
public:
    void push(T t)
    {
        this->push_back(t);
    }

    //移除栈顶对象，并作为函数的值 返回该对象。空，则返回null
    T pop()
    {
        if (this->size() <= 0)
        {
            return NULL;
        }

        T t = this->back();
        this->pop_back();

        return t;
    }

    //增加一个元索 
    void add(T t)
    {
        this->push(t);
    }


};