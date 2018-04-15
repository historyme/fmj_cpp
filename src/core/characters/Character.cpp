#include "Character.h"


Character::Character()
    :mWalkingSprite(NULL)
{
    mState = STATE_STOP;
    mDirection = South;
}

Character::~Character()
{
    if (NULL != mWalkingSprite)
    {
        delete mWalkingSprite;
    }
}

std::string Character::getName()
{
    return mName;
}

int Character::getCharacterState()
{
    return mState;
}

void Character::setCharacterState(int state)
{
    mState = state;
}

Point  Character::getPosInMap()
{
    return mPosInMap;
}

void Character::setPosInMap(int x, int y)
{
    mPosInMap.set(x, y);
}

Point Character::getPosOnScreen(Point *posMapScreen)
{
    return Point(mPosInMap.x - posMapScreen->x,
        mPosInMap.y - posMapScreen->y);
}

void Character::setPosOnScreen(Point *p, Point *posMapScreen)
{
    mPosInMap.set(p->x + posMapScreen->x, p->y + posMapScreen->y);
}

void Character::setPosOnScreen(int x, int y, Point *posMapScreen)
{
    mPosInMap.set(x + posMapScreen->x, y + posMapScreen->y);
}

Direction Character::getDirection()
{
    return mDirection;
}

void Character::setDirection(Direction d)
{
    mDirection = d;
    mWalkingSprite->setDirection(d);
}

void Character::setWalkingSprite(WalkingSprite *sprite)
{
    mWalkingSprite = sprite;
    mWalkingSprite->setDirection(getDirection());
}

void Character::walk(Direction d)
{
    if (d == getDirection())
    {
        mWalkingSprite->walk();
    }
    else
    {
        mWalkingSprite->walk(d);
        setDirection(d);
    }
    updatePosInMap(d);
}

void Character::updatePosInMap(Direction d)
{
    switch (d)
    {
    case East:
        mPosInMap.x++;
        break;
    case West:
        mPosInMap.x--;
        break;
    case North:
        mPosInMap.y--;
        break;
    case South:
        mPosInMap.y++;
        break;
    }
}

void Character::walkStay(Direction d)
{
    if (d == getDirection())
    {
        mWalkingSprite->walk();
    }
    else
    {
        mWalkingSprite->walk(d);
        setDirection(d);
    }
}

void Character::walkStay()
{
    mWalkingSprite->walk();
}

void Character::setStep(int step)
{
    mWalkingSprite->setStep(step);
}

int Character::getStep()
{
    return mWalkingSprite->getStep();
}

void Character::drawWalkingSprite(Canvas *canvas, Point *posMapScreen)
{
    Point *p = &(getPosOnScreen(posMapScreen));
    mWalkingSprite->draw(canvas, p->x * 16, p->y * 16);
}
