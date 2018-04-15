#ifndef _GoodsMedicine_H_
#define _GoodsMedicine_H_

#include <iostream>
#include "IEatMedicine.h"

class Player;
class DatLib;
class ResSrs;
class BaseGoods;
/**
 * 09药物类
 * 普通药物，任何人都可以用
 * @author Chen
 *
 */
class GoodsMedicine : public BaseGoods, public IEatMedicine
{
public:
    GoodsMedicine()
    {
        bInstanceof_GoodsMedicine = true;
    }

    virtual ~GoodsMedicine()
    {

    }

protected:
    virtual void setOtherData(char *buf, int offset)
    {
        mHp = get2BytesInt(buf, offset + 0x16);
        mMp = get2BytesInt(buf, offset + 0x18);
        mAniType = 2/*(int)buf[offset + 0x1b] & 0xff*/;
        mAniIndex=(int)buf[offset + 0x1a] & 0xff;
        mBitMask = (int)buf[offset + 0x1c] & 0xff;
    }

private:
    int mHp;
    int mMp;
    int mBitMask; // 治疗 毒、乱、封、眠

public:
    virtual void eat(Player *player)
    {
        player->setHP(player->getHP() + mHp);
        if (player->getHP() > player->getMaxHP())
        {
            player->setHP(player->getMaxHP());
        }
        player->setMP(player->getMP() + mMp);
        if (player->getMP() > player->getMaxMP())
        {
            player->setMP(player->getMaxMP());
        }
        player->delDebuff(mBitMask);
        player->sGoodsList->deleteGoods(mType, mIndex);
    }

    /* 是具有全体治疗效果 */
    virtual bool effectAll()
    {
        return (mBitMask & 0x10) != 0;
    }
};

#endif
