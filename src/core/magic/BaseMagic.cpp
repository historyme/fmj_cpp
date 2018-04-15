#include "BaseMagic.h"

BaseMagic::BaseMagic()
{
    bMagicRestore = false;
    mbInstanceof_MagicAttack = false;
    mbInstanceof_MagicSpecial = false;
    mMagicAni = NULL;
}

BaseMagic::~BaseMagic()
{
    if (NULL != mMagicAni)
    {
        delete mMagicAni;
    }
}

void BaseMagic::setData(char *buf, int offset)
{
    mType = (int)buf[offset] & 0xFF;
    mIndex = (int)buf[offset + 1] & 0xFF;
    mRound = (int)buf[offset + 3] & 0x7f;
    mIsForAll = ((int)buf[offset + 3] & 0x80) != 0;
    mCostMp = (int)buf[offset + 4];
    mMagicAni = (ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 2, (int)buf[offset + 5] & 0xFF);
    mMagicName = getString(buf, offset + 6);
    if (((int)buf[offset + 2] & 0xff) > 0x70)   // Ä§·¨ÃèÊö¹ý³¤
    {
        buf[offset + 0x70] = 0;
    }
    mMagicDescription = getString(buf, offset + 0x1a);
    setOtherData(buf, offset);
}

int BaseMagic::getRoundNum()
{
    return mRound;
}

bool BaseMagic::isForAll()
{
    return mIsForAll;
}

int BaseMagic::getCostMp()
{
    return mCostMp;
}

ResSrs * BaseMagic::getMagicAni()
{
    return mMagicAni;
}

std::string BaseMagic::getMagicName()
{
    return mMagicName;
}

std::string BaseMagic::getMagicDescription()
{
    return mMagicDescription;
}

void BaseMagic::use(FightingCharacter *src, FightingCharacter *dst)
{

}

