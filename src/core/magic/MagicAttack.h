#ifndef _MagicAttack_H_
#define _MagicAttack_H_

#include <iostream>
#include "BaseMagic.h"

class FightingCharacter;

/**
 * 01攻击型
 * @author Chen
 *
 */
class MagicAttack: public BaseMagic
{
protected :
    virtual void setOtherData(char *buf, int offset);

public :
    MagicAttack();
    virtual ~MagicAttack(){}
    virtual void use(FightingCharacter *src, FightingCharacter *dst);

    void use(FightingCharacter *src, std::vector<FightingCharacter *> &dst);

private:
    int mHp;//-8000~+8000，为正数时表示敌人损失生命的基数，为负数时表示从敌人身上吸取生命的基数
    int mMp;//-8000~+8000，为正数时表示敌人损失真气的基数，为负数时表示从敌人身上吸取真气的基数
    int mDf;//0~100，表示敌人的防御力减弱的百分比
    int mAt;//0~100，表示敌人的攻击力减弱的百分比
    int mBuff;//高四位 持续回合，低四位毒、乱、封、眠
    int mDebuff;//速 0~100，表示敌人的身法减慢的百分比

};

#endif
