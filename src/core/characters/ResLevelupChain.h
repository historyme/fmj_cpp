#ifndef _ResLevelupChain_H_
#define _ResLevelupChain_H_

#include <iostream>
#include "ResBase.h"

class ResLevelupChain: public  ResBase
{
public:
    ResLevelupChain();

    virtual ~ResLevelupChain();

    virtual void setData(char * buf, int offset);

    int getMaxLevel();

    int getMaxHP(int level);

    int getHP(int level);

    int getMaxMP(int l);

    int getMP(int l);

    int getAttack(int l);

    int getDefend(int l);

    int getNextLevelExp(int l);

    int getSpeed(int l);

    int getLingli(int l);

    int getLuck(int l);

    int getLearnMagicNum(int l);

private:
    static const int LEVEL_BYTES = 20; // 一个级别数据所占字节数
    int mMaxLevel; // 最高级别
    char* mLevelData; // 级别数据
};

#endif
