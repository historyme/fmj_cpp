#ifndef _GoodsDecorations_H_
#define _GoodsDecorations_H_

#include <iostream>
#include "GoodsEquipment.h"

class Player;
class DatLib;
class MagicAttack;

/**
 * 06饰品类
 * @author Chen
 *
 */
class GoodsDecorations: public GoodsEquipment
{
public:
    GoodsDecorations()
    {

    }

    virtual ~GoodsDecorations()
    {

    }
protected:
    virtual void setOtherData(char *buf, int offset)
    {
        mMp = get1ByteSInt(buf, offset + 0x16);
        mHp = get1ByteSInt(buf, offset + 0x17);
        mdf = get1ByteSInt(buf, offset + 0x18);
        mat = (int)buf[offset + 0x19] & 0xff;
        mlingli = get1ByteSInt(buf, offset + 0x1a);
        mSpeed = get1ByteSInt(buf, offset + 0x1b);
        mMagic = (int)buf[offset + 0x1c] & 0xff;
        mLuck = get1ByteSInt(buf, offset + 0x1d);
    }


public:
    virtual void putOn(Player *p)
    {
        GoodsEquipment::putOn(p);
        // TODO 每回合的恢复扣除
    }

    virtual void takeOff(Player *p)
    {
        GoodsEquipment::takeOff(p);
        // TODO 每回合的恢复扣除
    }

private:
    int mMp; // 表示战斗时，每回合恢复或扣除多少真气
    int mHp; // 表示战斗时，每回合恢复或扣除多少生命
    int mMagic; // 合体魔方序号

public :
    MagicAttack *getCoopMagic()
    {
        return (MagicAttack *)DatLib::GetRes(DatLib::RES_MRS, 1, mMagic);
    }
};
#endif
