#ifndef _Random_H_
#define _Random_H_

class Random
{
public:
    Random();
    Random(long seed);
    ~Random();

    //生成一个随机的int值
    //返回值 [0, val)
    int nextInt(int val);

    //生成一个随机的bool值
    //返回值 true false
    bool nextbool();
private:
    void init();

public:
    static bool bInit;

};

#endif