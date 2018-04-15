#ifndef _ResMagicChain_H_
#define _ResMagicChain_H_

#include <iostream>

#include "DatLib.h"
#include "ResBase.h"
#include "BaseMagic.h"

/**
 * 魔法链资源
 * @author Chen
 *
 */
class ResMagicChain: public ResBase
{
public:
    virtual void setData(char *buf, int offset);

    ResMagicChain();

    virtual ~ResMagicChain();
    /**
     * 返回已经学会的魔法数量
     * @return
     */
    int getLearnNum();

    void setLearnNum(int num);

    /**
     * 学会魔法数量加一
     */
    void learnNextMagic();

    /**
     * 得到魔法总数
     * @return
     */
    int getMagicSum();

    BaseMagic *getMagic(int index);   // TODO fix NULL;

private:
    int mNum; // 魔法数量

    std::vector<BaseMagic *> mMagics;
    int mLearnNum; // 学会的魔法数量
};

#endif
