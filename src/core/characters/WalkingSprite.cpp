#include "WalkingSprite.h"

WalkingSprite::WalkingSprite(int type, int id):
mOffset(1),
mI(0)
{
    mResImage = (ResImage *)DatLib::getInstance()->getRes(DatLib::RES_ACP, type, id);
}

WalkingSprite::~WalkingSprite()
{
    delete mResImage;
}

int WalkingSprite::getId()
{
    return mResImage->getIndex();
}

void WalkingSprite::setDirection(Direction d)
{
    switch (d)
    {
    case North:
        mOffset = 1;
        break;
    case East:
        mOffset = 4;
        break;
    case South:
        mOffset = 7;
        break;
    case West:
        mOffset = 10;
        break;
    }
}

void WalkingSprite::walk(Direction d)
{
    setDirection(d);
    walk();
}

void WalkingSprite::walk()
{
    ++mI;
    mI %= 4;
}

void WalkingSprite::setStep(int step)
{
    mI = step % 4;
}

int WalkingSprite::getStep()
{
    return mI;
}

void WalkingSprite::draw(Canvas *canvas, int x, int y)
{
    y = y + 16 - mResImage->getHeight();
    if (x + mResImage->getWidth() > 0 && x < 160 - 16 &&
        y + mResImage->getHeight() > 0 && y < 96)
    {
        mResImage->draw(canvas, mOffset + OFFSET[mI], x + MAP_LEFT_OFFSET, y);
    }
}

const int WalkingSprite::OFFSET[4] = { 0, 1, 2, 1 };

