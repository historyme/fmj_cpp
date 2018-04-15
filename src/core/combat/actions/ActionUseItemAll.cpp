#include "ActionUseItemAll.h"
#include "Player.h"

ActionUseItemAll::ActionUseItemAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, BaseGoods *goods)
    :ActionMultiTarget(attacker, targets), mState(STATE_PRE), mAni(NULL)
{
    this->goods = goods;
}

ActionUseItemAll::~ActionUseItemAll()
{

}

void ActionUseItemAll::preproccess()
{
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    //mAni = magic->getMagicAni(); TODO NULL fix
    //mAni->startAni();
    //mAni->setIteratorNum(2);
    mRaiseAnis.push_back(new RaiseAnimation(10, 20, 10, 0));
    mRaiseAnis.push_back(new RaiseAnimation(30, 10, 10, 0));
}

bool ActionUseItemAll::update(long delta)
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
//         if (!mAni->update(delta))
//         {
//             // 魔法动画完成
//             mState = STATE_AFT;
//             if (mAttacker->bInstanceof_Player)
//             {
//                 static_cast<Player*>(mAttacker)->setFrameByState();
//             }
//             else
//             {
//                 mAttacker->getFightingSprite()->move(-2, -2);
//             }
//         }
        break;

    case STATE_AFT:
        return updateRaiseAnimation(delta);
        break;
    }
    return true;
}

void ActionUseItemAll::draw(Canvas *canvas)
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
int ActionUseItemAll::getPriority()
{
    // TODO Auto-generated method stub
    return ActionMultiTarget::getPriority();
}