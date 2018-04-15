#include "Action.h"

Action::Action()
{
    mTimeCnt = 0;
    mCurrentFrame = 0;
    bInstanceof_ActionFlee = false;
    bInstanceof_ActionCoopMagic = false;
}

bool Action::update(long delta)
{
    mTimeCnt += delta;
    if (mTimeCnt >= DELTA)
    {
        ++mCurrentFrame;
        mTimeCnt = 0;
    }
    return true;
}

int Action::getPriority()
{
    return mAttacker->getSpeed();
}

bool Action::isAttackerAlive()
{
    return mAttacker->isAlive();
}
