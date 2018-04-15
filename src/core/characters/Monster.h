#ifndef _Monster_H_
#define _Monster_H_

#include <iostream>
#include "BaseGoods.h"
#include "DatLib.h"
#include "ResMagicChain.h"
#include "FightingCharacter.h"

class Monster: public FightingCharacter
{
public:
    Monster();
    virtual ~Monster();

    virtual void setData(char *buf, int offset);

    /**
     *
     * @param i 屏幕上的位置
     */
    void setOriginalCombatPos(int i);

    /**
     * 打怪得到的金钱
     * @return
     */
    int getMoney()
    {
        return mMoney;
    }

    /**
     * 打怪得到的经验
     * @return
     */
    int getExp()
    {
        return mEXP;
    }

    /**
     * 怪物掉落的物品
     * @return
     */
    BaseGoods *getDropGoods();


private:
    int mLastRound; // 异常状态持续回合
    int mIQ; // 智商，影响魔法使用率
    int mMoney; // 打挂怪物掉的钱
    int mEXP; // 打挂怪物得到的经验
    int mCarryGoods1[3]; // 能被偷走的 type id num
    int mCarryGoods2[3]; // 打挂掉落的 type id num

    static const int MONSTRTPOS[3][2];
};

#endif
