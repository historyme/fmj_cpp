#include "MagicAttack.h"
#include "FightingCharacter.h"

void MagicAttack::setOtherData(char *buf, int offset)
{
    mHp = get2BytesSInt(buf, offset + 0x12);
    mMp = get2BytesSInt(buf, offset + 0x14);
    mDf = (int)buf[offset + 0x16] & 0xff;
    mAt = (int)buf[offset + 0x17] & 0xff;
    mBuff = (int)buf[offset + 0x18] & 0xff;
    mDebuff = (int)buf[offset + 0x19] & 0xff;
}

MagicAttack::MagicAttack()
{
    mbInstanceof_MagicAttack = true;
}

void MagicAttack::use(FightingCharacter *src, FightingCharacter *dst)
{
    src->setMP(src->getMP() - getCostMp());
    dst->setHP(dst->getHP() - mHp);
}

void MagicAttack::use(FightingCharacter *src, std::vector<FightingCharacter *> &dst)
{
    src->setMP(src->getMP() - getCostMp());
    for (int i = 0; i < (int)(dst.size()); i++)
    {
        dst[i]->setHP(dst[i]->getHP() - mHp);
    }
}
