#include "ActionPhysicalAttackOne.h"
#include "FightingCharacter.h"
#include "Player.h"

ActionPhysicalAttackOne::ActionPhysicalAttackOne(FightingCharacter *attacker, FightingCharacter *target)
    :ActionSingleTarget(attacker, target), mTotalMark(true)
{
}

ActionPhysicalAttackOne::~ActionPhysicalAttackOne()
{

}

void ActionPhysicalAttackOne::preproccess()
{
    // TODO 记下伤害值、异常状态
    int damage;
    ox = mAttacker->getCombatX();
    oy = mAttacker->getCombatY();
    dx = (float)(mTarget->getCombatX() - mAttacker->getCombatX()) / TOTAL_FRAME;
    dy = (float)(mTarget->getCombatY() - mAttacker->getCombatY()) / TOTAL_FRAME;
    damage = mAttacker->getAttack() - mTarget->getDefend();
    if (damage <= 0)
    {
        damage = 1;
    }
    if (mAttacker->bInstanceof_Player)
    {
        damage *= 10;
    }
    damage += random.nextInt(10);
    mTarget->setHP(mTarget->getHP() - damage);
    mRaiseAni = new RaiseAnimation(mTarget->getCombatLeft(), mTarget->getCombatTop(), -damage, 0);
}

bool ActionPhysicalAttackOne::update(long delta)
{
    ActionSingleTarget::update(delta);
    if (mCurrentFrame < TOTAL_FRAME)
    {
        // 发起动作
         mAttacker->setCombatPos((int)(ox + dx * mCurrentFrame), (int)(oy + dy * mCurrentFrame));
        if (mAttacker->bInstanceof_Monster)
        {
            FightingSprite *fs =  mAttacker->getFightingSprite();
            fs->setCurrentFrame(fs->getFrameCnt() * mCurrentFrame / TOTAL_FRAME + 1);
        }
        else if (mAttacker->bInstanceof_Player)
        {
            FightingSprite *fs =  mAttacker->getFightingSprite();
            fs->setCurrentFrame(5 * mCurrentFrame / TOTAL_FRAME + 1);
        }
    }
    else if (mCurrentFrame > TOTAL_FRAME)
    {
        // 扣血、异常状态的动画
        if (!updateRaiseAnimation(delta))
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
    }
    else if (mTotalMark)
    {
        mTotalMark = false;
         mAttacker->setCombatPos(ox, oy);
         if (mAttacker->bInstanceof_Monster)
        {
            FightingSprite *fs =  mAttacker->getFightingSprite();
            fs->setCurrentFrame(1);
        }
         else if (mAttacker->bInstanceof_Player)
        {
            (static_cast<Player*>(mAttacker))->setFrameByState();
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
    return true;
}

void ActionPhysicalAttackOne::draw(Canvas *canvas)
{
    if (mCurrentFrame >= TOTAL_FRAME)
    {
        drawRaiseAnimation(canvas);
    }
}

int ActionPhysicalAttackOne::getPriority()
{
    // TODO Auto-generated method stub
    return ActionSingleTarget::getPriority();
}
