#include "ResMagicChain.h"




void ResMagicChain::setData(char *buf, int offset)
{
    mType = (int)buf[offset] & 0xff;
    mIndex = (int)buf[offset + 1] & 0xff;
    mNum = (int)buf[offset + 2] & 0xff;

    int index = offset + 3;
    mMagics.resize(mNum);
    for (int i = 0; i < mNum; i++)
    {
        int magicType = (int)buf[index++];
        int magicIndex = (int)buf[index++];

        mMagics[i] = (BaseMagic *)DatLib::GetRes(DatLib::RES_MRS, magicType, magicIndex);
    }
}

ResMagicChain::ResMagicChain()
{
    mLearnNum = 0;
}

ResMagicChain::~ResMagicChain()
{
    for (int i = 0; i < (int)(mMagics.size()); i++)
    {
        delete mMagics[i];
    }
    mMagics.clear();
}

int ResMagicChain::getLearnNum()
{
    return mLearnNum;
}

void ResMagicChain::setLearnNum(int num)
{
    mLearnNum = num;
}

void ResMagicChain::learnNextMagic()
{
    ++mLearnNum;
}

int ResMagicChain::getMagicSum()
{
    return mNum;
}

BaseMagic * ResMagicChain::getMagic(int index) // TODO fix NULL
{
    return mMagics[index];
}
