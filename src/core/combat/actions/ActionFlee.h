#ifndef _ActionFlee_h_
#define _ActionFlee_h_

#include <iostream>

#include "Action.h"
#include "player.h"
#include "Canvas.h"
#include "Runnable.h"

class ActionFlee : public Action
{

private:
    static const int FRAME_CNT = 5;
    Player *player;
    bool fleeSucceed;
    Runnable *runAfterFlee;
    int ox;
    int oy;
    int dy;

public:
    /**
     *
     * @param p 逃跑者
     * @param fleeSuc 是否逃跑成功
     * @param runAft 逃跑动作完毕后，执行之
     */
    ActionFlee(Player *p, bool fleeSuc, Runnable *runAft)
    {
        bInstanceof_ActionFlee = true;
        player = p;
        fleeSucceed = fleeSuc;
        runAfterFlee = runAft;
    }

    virtual void preproccess()
    {
        // TODO calc the pos
        ox = player->getCombatX();
        oy = player->getCombatY();
        dy = (96 - oy) / FRAME_CNT;
        player->getFightingSprite()->setCurrentFrame(1);
    }

    virtual bool update(long delta)
    {
        Action::update(delta);
        if (mCurrentFrame < FRAME_CNT)
        {
            player->setCombatPos(ox, oy + dy * mCurrentFrame);
            return true;
        }
        else if (!fleeSucceed && mCurrentFrame < FRAME_CNT + 2)
        {
            player->setCombatPos(ox, oy);
            player->getFightingSprite()->setCurrentFrame(11);
        }
        else if (!fleeSucceed)
        {
            player->setFrameByState();
        }
        return false;
    }

    virtual void postExecute()
    {
        // TODO Auto-generated method stub
        if (fleeSucceed && runAfterFlee != NULL)
        {
            runAfterFlee->run();
        }
        else
        {
            player->setCombatPos(ox, oy);
        }
    }


protected:
    virtual bool updateRaiseAnimation(long delta)
    {
        // TODO Auto-generated method stub
        return false;
    }

    virtual void drawRaiseAnimation(Canvas *canvas)
    {
        // TODO Auto-generated method stub

    }
public:
    virtual int getPriority()
    {
        return player->getSpeed() * 100;
    }
    virtual bool isAttackerAlive()
    {
        return true;
    }

    virtual bool isTargetAlive()
    {
        // TODO Auto-generated method stub
        return false;
    }
    virtual bool isTargetsMoreThanOne()
    {
        // TODO Auto-generated method stub
        return false;
    }

    virtual bool targetIsMonster()
    {
        // TODO Auto-generated method stub
        return false;
    }

};

#endif
