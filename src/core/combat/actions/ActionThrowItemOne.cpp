#include "ActionThrowItemOne.h"
#include "GoodsHiddenWeapon.h"
#include "Player.h"

ActionThrowItemOne::ActionThrowItemOne(FightingCharacter *attacker, FightingCharacter *target, GoodsHiddenWeapon *g)
    :ActionSingleTarget(attacker, target), mState(STATE_PRE), mAni(NULL), hiddenWeapon(g)
{
}

ActionThrowItemOne::~ActionThrowItemOne()
{

}

void ActionThrowItemOne::preproccess()
{
    // TODO 记下伤害值、异常状态 there is NULL pointer
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    mAni = hiddenWeapon->getAni();
    mAni->startAni();
    mAni->setIteratorNum(2);
    mAniX = mTarget->getCombatX();
    mAniY = mTarget->getCombatY();

    int affectHp = hiddenWeapon->getAffectHp();

    //这里将物品数量减一，只是为了展示用的
    hiddenWeapon->setGoodsNum(hiddenWeapon->getGoodsNum() - 1);

    mRaiseAni = new RaiseAnimation(mAniX, mTarget->getCombatTop(), -1 * abs(affectHp), 0);
}

bool ActionThrowItemOne::update(long delta)
{
    ActionSingleTarget::update(delta);
    switch (mState)
    {
    case STATE_PRE:
        if (mCurrentFrame < 10)
        {
            if (mAttacker->bInstanceof_Player)
            {
                mAttacker->getFightingSprite()->setCurrentFrame(
                    mCurrentFrame * 3 / 10 + 6);
            }
            else
            {
                mAttacker->setCombatPos(ox + 2, oy + 2);
            }
        }
        else
        {
            mState = STATE_ANI;
        }
        break;

    case STATE_ANI:
        if (!mAni->update(delta))
        {
            mState = STATE_AFT;
            if (mAttacker->bInstanceof_Player)
            {
                static_cast<Player*>(mAttacker)->setFrameByState();
            }
            else
            {
                mAttacker->getFightingSprite()->move(-2, -2);
            }
            if (mTarget->bInstanceof_Player)
            {
                mTarget->getFightingSprite()->setCurrentFrame(10);
            }
            else
            {
                mTarget->getFightingSprite()->move(2, 2);
            }
        }
        break;

    case STATE_AFT:
        if (!mRaiseAni->update(delta))
        {
            if (mTarget->bInstanceof_Player)
            {
                static_cast<Player*>(mTarget)->setFrameByState();
            }
            else
            {
                mTarget->getFightingSprite()->move(-2, -2);
            }
            return false;
        }
        break;
    }
    return true;
}

void ActionThrowItemOne::draw(Canvas *canvas)
{
    if (mState == STATE_ANI)
    {
        mAni->drawAbsolutely(canvas, mAniX, mAniY);
    }
    else if (mState == STATE_AFT)
    {
        mRaiseAni->draw(canvas);
    }
}

