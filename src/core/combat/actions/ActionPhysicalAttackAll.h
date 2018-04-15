#ifndef _ActionPhysicalAttackAll_H_
#define _ActionPhysicalAttackAll_H_

#include "ActionMultiTarget.h"
#include "Random.h"
#include "RaiseAnimation.h"
#include "FightingCharacter.h"

class  ActionPhysicalAttackAll : public ActionMultiTarget
{
public:
    ActionPhysicalAttackAll(FightingCharacter *attacker, vector <FightingCharacter *> targets);
    virtual ~ActionPhysicalAttackAll();
    virtual void preproccess()
    {
        // TODO 记下伤害值、异常状态
        int damage;
        ox = mAttacker->getCombatX();
        oy = mAttacker->getCombatY();
        dx = (44.0f - mAttacker->getCombatX()) / TOTAL_FRAME;
        dy = (14.0f - mAttacker->getCombatY()) / TOTAL_FRAME;
        for (int i = 0; i < mTargets.size(); i++)
        {
            FightingCharacter *fc = mTargets.at(i);
            damage = mAttacker->getAttack() - fc->getDefend();
            if (damage <= 0)
            {
                damage = 1;
            }
            damage += random.nextInt(3);
            fc->setHP(fc->getHP() - damage);
            mRaiseAnis.push_back(new RaiseAnimation(mTargets.at(i)->getCombatX(), mTargets.at(i)->getCombatY(), -damage, 0));
        }
    }

    virtual bool update(long delta)
    {
        ActionMultiTarget::update(delta);
        if (mCurrentFrame < TOTAL_FRAME)
        {
            // 发起动作
            mAttacker->setCombatPos((int)(ox + dx * mCurrentFrame), (int)(oy + dy * mCurrentFrame));
            if (mAttacker->bInstanceof_Monster)
            {
                FightingSprite *fs = mAttacker->getFightingSprite();
                fs->setCurrentFrame(fs->getFrameCnt() * mCurrentFrame / TOTAL_FRAME + 1);
            }
            else if (mAttacker->bInstanceof_Player)
            {
                FightingSprite *fs = mAttacker->getFightingSprite();
                fs->setCurrentFrame(5 * mCurrentFrame / TOTAL_FRAME + 1);
            }
        }
        else if (mCurrentFrame > TOTAL_FRAME)
        {
            // 扣血、异常状态的动画
            return updateRaiseAnimation(delta);
        }
        else
        {
            mAttacker->setCombatPos(ox, oy);
            if (mAttacker->bInstanceof_Monster)
            {
                FightingSprite *fs = mAttacker->getFightingSprite();
                fs->setCurrentFrame(1);
            }
            else if (mAttacker->bInstanceof_Player)
            {
                FightingSprite *fs = mAttacker->getFightingSprite();
                fs->setCurrentFrame(1); // TODO the old state
            }
        }
        return true;
    }

    virtual void draw(Canvas *canvas)
    {
        if (mCurrentFrame >= TOTAL_FRAME)
        {
            drawRaiseAnimation(canvas);
        }
    }

    virtual int getPriority()
    {
        // TODO Auto-generated method stub
        return ActionMultiTarget::getPriority();
    }

private:
    static const int TOTAL_FRAME = 5;
    float dx, dy;
    int ox, oy;
    int buffRound;
    Random random;

};

#endif