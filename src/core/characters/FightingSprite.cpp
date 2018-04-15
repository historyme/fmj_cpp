#include "FightingSprite.h"


FightingSprite::FightingSprite(int resType, int index)
{
    mCurrentFrame = 1;
    mImage = NULL;

    if (resType == DatLib::RES_ACP)   // 怪物的
    {
        mImage = (ResImage *)DatLib::GetRes(DatLib::RES_ACP, 3, index);
    }
    else if (resType == DatLib::RES_PIC)     // 玩家角色的
    {
        mImage = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 3, index);
    }
    else
    {
        printf("resType 有错.");
    }
}

FightingSprite::~FightingSprite()
{
    if (NULL != mImage)
    {
        delete mImage;
    }
}

void FightingSprite::draw(Canvas *canvas)
{
    mImage->draw(canvas, mCurrentFrame, mCombatX - mImage->getWidth() / 2,
        mCombatY - mImage->getHeight() / 2);
}

void FightingSprite::draw(Canvas *canvas, int x, int y)
{
    mImage->draw(canvas, mCurrentFrame, x, y);
}

void FightingSprite::setCombatPos(int x, int y)
{
    mCombatX = x;
    mCombatY = y;
}

void FightingSprite::move(int dx, int dy)
{
    mCombatX += dx;
    mCombatY += dy;
}

int FightingSprite::getCombatX()
{
    return mCombatX;
}

int FightingSprite::getCombatY()
{
    return mCombatY;
}

int FightingSprite::getWidth()
{
    return mImage->getWidth();
}

int FightingSprite::getHeight()
{
    return mImage->getHeight();
}

int FightingSprite::getCurrentFrame()
{
    return mCurrentFrame;
}

void FightingSprite::setCurrentFrame(int i)
{
    mCurrentFrame = i;
}

int FightingSprite::getFrameCnt()
{
    return mImage->getNumber();
}
