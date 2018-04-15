#ifndef _GoodsWeapon_H_
#define _GoodsWeapon_H_

#include "Player.h"


/**
 * 07武器类
 * @author Chen
 *
 */
class GoodsWeapon: public GoodsEquipment
{
public:
    GoodsWeapon()
    {
        bFatherOfGoodsWeapon = true;
    }

    virtual ~GoodsWeapon(){}

    virtual void putOn(Player *p)
    {
        GoodsEquipment::putOn(p);
        p->addAtbuff(mBitEffect, getSumRound());
    }

    virtual  void takeOff(Player *p)
    {
        GoodsEquipment::takeOff(p);
        p->delAtbuff(mBitEffect);
    }

    bool attackAll()
    {
        return (mBitEffect & 0x10) != 0;
    }
};

#endif
