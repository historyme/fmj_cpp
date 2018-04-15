#ifndef _BaseMagic_H_
#define _BaseMagic_H_

#include <iostream>
#include "DatLib.h"
#include "ResBase.h"
#include "ResSrs.h"

class FightingCharacter;

class BaseMagic: public ResBase
{
public:
    BaseMagic();
    virtual ~BaseMagic();
    virtual  void setData(char *buf, int offset);

    /**
     * 获取魔法的持续回合
     * @return
     */
    int getRoundNum();


    /**
     * 魔方是否影响全体
     * @return
     */
    bool isForAll();

    /**
     * 魔方耗费真气
     * @return
     */
    int getCostMp();



    /**
     * 战斗中使用魔法时播放的动画
     * @return
     */
    ResSrs *getMagicAni();

    std::string getMagicName();

    std::string getMagicDescription();

    virtual void use(FightingCharacter *src, FightingCharacter *dst);

    virtual int getAffectHp(){ return 0; }

protected:
    virtual void setOtherData(char *buf, int offset) = 0;

private:
    int mRound; // 持续回合
    int mCostMp; // 耗费真气
    bool mIsForAll; // 是否影响全体
    ResSrs *mMagicAni; // 魔法动画
    std::string mMagicDescription;  // 魔方描述
    std::string mMagicName; // 魔法名
public:
    bool bMagicRestore;
    bool mbInstanceof_MagicAttack;
    bool mbInstanceof_MagicSpecial;
};

#endif
