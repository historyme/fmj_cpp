#include "BaseGoods.h"


BaseGoods::BaseGoods()
{
    mGoodsNum = 0;
    mImage = NULL;
    bInstanceof_GoodsMedicine = false;
}

BaseGoods::~BaseGoods()
{
    if (NULL != mImage)
    {
        delete mImage;
    }
}

void BaseGoods::setData(char * buf, int offset)
{
    mType = (int)buf[offset] & 0xFF;
    mIndex = (int)buf[offset + 1] & 0xFF;
    mEnable = (int)buf[offset + 3] & 0xFF;
    mSumRound = (int)buf[offset + 4] & 0xff;
    mImage = (ResImage *)DatLib::GetRes(DatLib::RES_GDP, mType, (int)buf[offset + 5] & 0xff);
    mName = getString(buf, offset + 6);
    mBuyPrice = get2BytesInt(buf, offset + 0x12);
    mSellPrice = get2BytesInt(buf, offset + 0x14);
    mDescription = getString(buf, offset + 0x1e);
    mEventId = get2BytesInt(buf, offset + 0x84);
    setOtherData(buf, offset);
}

bool BaseGoods::canPlayerUse(int playId)
{
    if (playId >= 1 && playId <= 4)
    {
        return (mEnable & (1 << (playId - 1))) != 0;
    }
    return false;
}

int BaseGoods::getSumRound()
{
    return mSumRound;
}

void BaseGoods::draw(Canvas *canvas, int x, int y)
{
    mImage->draw(canvas, 1, x, y);
}

std::string BaseGoods::getName()
{
    return mName;
}

int BaseGoods::getBuyPrice()
{
    return mBuyPrice;
}

int BaseGoods::getSellPrice()
{
    return mSellPrice;
}

std::string BaseGoods::getDescription()
{
    return mDescription;
}

int BaseGoods::getEventId()
{
    return mEventId;
}

int BaseGoods::getGoodsNum()
{
    return mGoodsNum;
}

void BaseGoods::setGoodsNum(int num)
{
    mGoodsNum = num;
}

void BaseGoods::addGoodsNum(int d)
{
    mGoodsNum += d;
}

bool BaseGoods::effectAll()
{
    return false;
}
