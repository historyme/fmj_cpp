#include "ResLevelupChain.h"




ResLevelupChain::ResLevelupChain()
{

}

ResLevelupChain::~ResLevelupChain()
{
    delete mLevelData;
}

void ResLevelupChain::setData(char * buf, int offset)
{
    mType = (int)buf[offset] & 0xff;
    mIndex = (int)buf[offset + 1] & 0xff;
    mMaxLevel = (int)buf[offset + 2] & 0xff;

    mLevelData = new char[mMaxLevel * LEVEL_BYTES];
    memcpy(mLevelData, buf + offset + 4, mMaxLevel * LEVEL_BYTES);
}

int ResLevelupChain::getMaxLevel()
{
    return mMaxLevel;
}

int ResLevelupChain::getMaxHP(int level)
{
    if (level <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, level * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getHP(int level)
{
    if (level <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 2 + level * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getMaxMP(int l)
{
    if (l <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 4 + l * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getMP(int l)
{
    if (l <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 6 + l * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getAttack(int l)
{
    if (l <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 8 + l * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getDefend(int l)
{
    if (l <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 10 + l * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getNextLevelExp(int l)
{
    if (l <= mMaxLevel)
    {
        return get2BytesInt(mLevelData, 14 + l * LEVEL_BYTES - LEVEL_BYTES);
    }
    return 0;
}

int ResLevelupChain::getSpeed(int l)
{
    if (l <= mMaxLevel)
    {
        return (int)mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 16] & 0xff;
    }
    return 0;
}

int ResLevelupChain::getLuck(int l)
{
    if (l <= mMaxLevel)
    {
        return (int)mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 18] & 0xff;
    }
    return 0;
}

int ResLevelupChain::getLingli(int l)
{
    if (l <= mMaxLevel)
    {
        return (int)mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 17] & 0xff;
    }
    return 0;
}

int ResLevelupChain::getLearnMagicNum(int l)
{
    if (l <= mMaxLevel)
    {
        return (int)mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 19] & 0xff;
    }
    return 0;
}

