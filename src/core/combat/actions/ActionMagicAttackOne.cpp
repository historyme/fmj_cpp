#include "ActionMagicAttackOne.h"
#include "MagicAttack.h"
#include "FightingCharacter.h"
#include "BaseMagic.h"
#include "Player.h"

ActionMagicAttackOne::ActionMagicAttackOne(FightingCharacter *attacker, FightingCharacter *target, BaseMagic *magic)
    :ActionSingleTarget(attacker, target), mState(STATE_PRE)
{
    this->magic = magic;
}

ActionMagicAttackOne::~ActionMagicAttackOne()
{

}

void ActionMagicAttackOne::preproccess()
{
    // TODO 记下伤害值、异常状态
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    mAni = magic->getMagicAni();
    mAni->startAni();
    mAni->setIteratorNum(2);
    int ohp = mTarget->getHP();
    if (magic->mbInstanceof_MagicAttack)
    {
        (static_cast<MagicAttack*>(magic))->use(mAttacker, mTarget);
    }
    mAniX = mTarget->getCombatX();
    mAniY = mTarget->getCombatY() - mTarget->getFightingSprite()->getHeight() / 2;
    mRaiseAni = new RaiseAnimation(mTarget->getCombatX(), mTarget->getCombatY(), mTarget->getHP() - ohp, 0/*FightingCharacter.BUFF_MASK_DU*/);
}

bool ActionMagicAttackOne::update(long delta)
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
                (static_cast<Player*>(mAttacker))->setFrameByState();
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
                
                (static_cast<Player*>(mTarget))->setFrameByState();
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


void ActionMagicAttackOne::draw(Canvas *canvas)
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
