#ifndef _MagicRestore_H_
#define _MagicRestore_H_

#include <iostream>
#include "BaseMagic.h"
#include "FightingCharacter.h"

/**
 * 03恢复型
 * @author Chen
 *
 */
class MagicRestore: public  BaseMagic
{
public:
    MagicRestore()
    {
        bMagicRestore = true;
    }
    virtual ~MagicRestore(){}

protected:
    virtual void setOtherData(char *buf, int offset)
    {
        mHp = get2BytesInt(buf, offset + 0x12);
        mBuff = buf[offset + 0x18];
    }

private:
    int mHp; //  0~8000，表示被施展者恢复生命的数值。
    int mBuff; // 低四位，毒、乱、封、眠 是否具有医疗相应异常状态的能力

public:
    virtual void use(FightingCharacter *src, FightingCharacter *dst)
    {
        if (src->getMP() < getCostMp()) return;

        src->setMP(src->getMP() - getCostMp());

        dst->setHP(dst->getHP() + mHp);
        if (dst->getHP() > dst->getMaxHP())
        {
            dst->setHP(dst->getMaxHP());
        }
        dst->delDebuff(mBuff);
    }

    virtual int getAffectHp()
    {
        return mHp;
    }
};

#endif
