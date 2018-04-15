#include "ActionThrowItemAll.h"
#include "GoodsHiddenWeapon.h"
#include "Player.h"

ActionThrowItemAll::ActionThrowItemAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, GoodsHiddenWeapon *goods)
    :ActionMultiTarget(attacker, targets), mState(STATE_PRE), hiddenWeapon(goods)
{

}

ActionThrowItemAll::~ActionThrowItemAll()
{

}

void ActionThrowItemAll::preproccess()
{
    // TODO 记下伤害值、异常状态
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    mAni = hiddenWeapon->getAni();
    mAni->startAni();
    mAni->setIteratorNum(2);
    // TODO effect it
    mRaiseAnis.push_back(new RaiseAnimation(10, 20, 10, 0));
    mRaiseAnis.push_back(new RaiseAnimation(30, 10, 10, 0));
}

bool ActionThrowItemAll::update(long delta)
{
    ActionMultiTarget::update(delta);
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
            if (!targetIsMonster())
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {
                    FightingCharacter *fc = *iter;
                    fc->getFightingSprite()->setCurrentFrame(10);
                }
            }
            else
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {
                    FightingCharacter *fc = *iter;
                    fc->getFightingSprite()->move(2, 2);
                }
            }
        }
        break;

    case STATE_AFT:
        if (!updateRaiseAnimation(delta))
        {
            if (targetIsMonster())
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {
                    FightingCharacter *fc = *iter;
                    fc->getFightingSprite()->move(-2, -2);
                }
            }
            else
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {
                    FightingCharacter *fc = *iter;
                    (static_cast<Player*>(fc))->setFrameByState();
                }
            }
            return false;
        }
        break;
    }
    return true;
}

void ActionThrowItemAll::draw(Canvas *canvas)
{
    if (mState == STATE_ANI)
    {
        mAni->draw(canvas, 0, 0);
    }
    else if (mState == STATE_AFT)
    {
        drawRaiseAnimation(canvas);
    }
}
