#include "ActionMagicAttackAll.h"
#include "MagicAttack.h"
#include "Player.h"
#include "FightingCharacter.h"
#include "FightingSprite.h"

ActionMagicAttackAll::ActionMagicAttackAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, MagicAttack *magic)
    :ActionMultiTarget(attacker, targets), mState(STATE_PRE)
{
    this->magic = magic;
}

ActionMagicAttackAll::~ActionMagicAttackAll()
{

}

void ActionMagicAttackAll::preproccess()
{
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    mAni = magic->getMagicAni();
    mAni->startAni();
    mAni->setIteratorNum(2);
    magic->use(mAttacker, mTargets);
    mRaiseAnis.push_back(new RaiseAnimation(10, 10, 10, 0));
    mRaiseAnis.push_back(new RaiseAnimation(30, 10, 20, 0/*FightingCharacter.BUFF_MASK_DU*/));
}

bool ActionMagicAttackAll::update(long delta)
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
                (static_cast<Player*>(mAttacker))->setFrameByState();
            }
            else
            {
                mAttacker->getFightingSprite()->move(-2, -2);
            }
            if (mTargets.at(0)->bInstanceof_Player)
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
            if (mTargets.at(0)->bInstanceof_Player)
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {            
                    FightingCharacter *fc = *iter;
                    (static_cast<Player*>(fc))->setFrameByState();
                }
            }
            else
            {
                vector<FightingCharacter*>::const_iterator iter = mTargets.begin();
                for (; iter != mTargets.end(); ++iter)
                {
                    FightingCharacter *fc = *iter;
                    (static_cast<Player*>(fc))->getFightingSprite()->move(-2, -2);
                }
            }
            return false;
        }
        break;
    }
    return true;
}

void ActionMagicAttackAll::draw(Canvas *canvas)
{
    ActionMultiTarget::draw(canvas);
    if (mState == STATE_ANI)
    {
        mAni->draw(canvas, 0, 0);
    }
    else if (mState == STATE_AFT)
    {
        drawRaiseAnimation(canvas);
    }
}

int ActionMagicAttackAll::getPriority()
{
    return ActionMultiTarget::getPriority();
}
