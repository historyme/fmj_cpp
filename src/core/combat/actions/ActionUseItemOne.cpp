#include "ActionUseItemOne.h"
#include "GoodsMedicine.h"

ActionUseItemOne::ActionUseItemOne(FightingCharacter *attacker, FightingCharacter *target, BaseGoods *g)
    :ActionSingleTarget(attacker, target), mState(STATE_PRE), mAni(NULL), goods(g)
{

}

ActionUseItemOne::~ActionUseItemOne()
{
    if (mAni != NULL)
    {
        delete mAni;
    }
}

void ActionUseItemOne::preproccess()
{
    // TODO 记下伤害值、异常状态
    int hp = 0;
    if (goods->bInstanceof_GoodsMedicine)
    {
        mAni = (ResSrs*)DatLib::GetRes(DatLib::RES_SRS, goods->getAniType(), goods->getAniIndex());
        hp = mTarget->getHP();
        static_cast<GoodsMedicine*>(goods)->eat(static_cast<Player*>(mTarget));
        hp = mTarget->getHP() - hp;
    }
    else
    {
        mAni = (ResSrs*)DatLib::GetRes(DatLib::RES_SRS, 2, 1);
    }
    mAni->startAni();
    mAni->setIteratorNum(2);
    mAnix = mTarget->getCombatX();
    mAniy = mTarget->getCombatY();
    mRaiseAni = new RaiseAnimation(mTarget->getCombatX(), mTarget->getCombatTop(), hp, 0);
}

bool ActionUseItemOne::update(long delta)
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
            // 魔法动画完成
            mState = STATE_AFT;
            if (mAttacker->bInstanceof_Player)
            {
                static_cast<Player*>(mAttacker)->setFrameByState();
            }
            else
            {
                mAttacker->getFightingSprite()->move(-2, -2);
            }
        }
        break;

    case STATE_AFT:
        return mRaiseAni->update(delta);
        //          break;
    }
    return TRUE;
}

void ActionUseItemOne::draw(Canvas *canvas)
{
    if (mState == STATE_ANI)
    {
        mAni->drawAbsolutely(canvas, mAnix, mAniy);
    }
    else if (mState == STATE_AFT)
    {
        mRaiseAni->draw(canvas);
    }
}
