#ifndef _GoodsHiddenWeapon_H_
#define _GoodsHiddenWeapon_H_

#include <iostream>
#include "BaseGoods.h"
#include "Player.h"
#include "FightingCharacter.h"

class DatLib;
class ResSrs;

/**
 * 08暗器
 * @author Chen
 *
 */
class GoodsHiddenWeapon: public  BaseGoods
{
public:
    GoodsHiddenWeapon()
    {
        mAni = NULL;
    }

    virtual ~GoodsHiddenWeapon()
    {
        if (NULL != mAni)
        {
            delete mAni;
        }
    }

private:
    int get2ByteSint(char *buf, int start)
    {
        int i = ((int)buf[start] & 0xFF) | ((int)buf[start + 1] << 8 & 0x7F00);
        if (((int)buf[start + 1] & 0x80) != 0)
        {
            return -i;
        }
        return i;
    }
protected:
    virtual void setOtherData(char *buf, int offset)
    {
        mHp = get2ByteSint(buf, offset + 0x16);
        mMp = get2ByteSint(buf, offset + 0x18);
        mAni = (ResSrs *)DatLib::GetRes(DatLib::RES_SRS, (int)buf[offset + 0x1b] & 0xff,
                                        (int)buf[offset + 0x1a] & 0xff);
        mBitMask = (int)buf[offset + 0x1c] & 0xff;
    }

private:
    int mHp; // 当该值为正时表示敌人损失多少生命，为负时表示从敌人身上吸取多少生命到投掷者身上
    int mMp; // 当该值为正时表示敌人损失多少真气，为负时表示从敌人身上吸取多少真气到投掷者身上
    ResSrs *mAni;
    int mBitMask; // 000 全体否 毒乱封眠

public:
    /**
     *
     * @return 当该值为正时表示敌人损失多少生命，
     * 为负时表示从敌人身上吸取多少生命到投掷者身上
     */
    int getAffectHp()
    {
        return mHp;
    }

    /**
     *
     * @return 当该值为正时表示敌人损失多少真气，
     * 为负时表示从敌人身上吸取多少真气到投掷者身上
     */
    int getAffectMp()
    {
        return mMp;
    }

    ResSrs *getAni()
    {
        return mAni;
    }

    virtual bool effectAll()
    {
        return (mBitMask & 0x10) != 0;
    }

    void use(FightingCharacter *attacker, FightingCharacter *target)
    {
        int affectHp = getAffectHp();
        if (affectHp > 0)
        {
            target->setHP(target->getHP() - affectHp);
        }
        else if (affectHp < 0)
        {
            //吸血
            target->setHP(target->getHP() - abs(affectHp));
            attacker->setHP(attacker->getHP() + abs(affectHp));
        }
        else
        {
            //nothing
        }

        int affectMp = getAffectMp();
        if (affectMp > 0)
        {
            target->setHP(target->getHP() - affectMp);
        }
        else if (affectMp < 0)
        {
            //吸蓝
            target->setHP(target->getHP() - abs(affectMp));
            attacker->setHP(attacker->getHP() + abs(affectMp));
        }
        else
        {
            //nothing
        }

        //攻击方是玩家，则需要将物品减少1
        if (attacker->bInstanceof_Player)
        {
            Player::sGoodsList->useGoodsNum(getType(), getIndex(), 1);
        }
    }
};

#endif
