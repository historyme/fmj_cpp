#include "ActionSingleTarget.h"

ActionSingleTarget::ActionSingleTarget(FightingCharacter *attacker, FightingCharacter *target)
{
    mRaiseAni = NULL;
    mAttacker = attacker;
    mTarget = target;
}

ActionSingleTarget::~ActionSingleTarget()
{
    if (NULL != mRaiseAni)
    {
        delete mRaiseAni;
    }
}

void ActionSingleTarget::postExecute()
{
    mTarget->setVisiable(mTarget->isAlive());
}

bool ActionSingleTarget::updateRaiseAnimation(long delta)
{
    return mRaiseAni != NULL && mRaiseAni->update(delta);
}

void ActionSingleTarget::drawRaiseAnimation(Canvas *canvas)
{
    if (mRaiseAni != NULL)
    {
        mRaiseAni->draw(canvas);
    }
}

bool ActionSingleTarget::isTargetAlive()
{
    return mTarget->isAlive();
}

bool ActionSingleTarget::isTargetsMoreThanOne()
{
    return false;
}

bool ActionSingleTarget::targetIsMonster()
{
    return mTarget->bInstanceof_Monster;
}

void ActionSingleTarget::setTarget(FightingCharacter *fc)
{
    mTarget = fc;
}
