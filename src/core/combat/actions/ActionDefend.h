#pragma once

#include "ActionSingleTarget.h"

class FightingCharacter;
class Canvas;

class  ActionDefend : public ActionSingleTarget
{

public:
    ActionDefend(FightingCharacter *fc):
        ActionSingleTarget(fc,NULL)
    {
    }

    virtual void preproccess()
    {
    }

    virtual void postExecute()
    {
    }

    virtual bool isTargetAlive()
    {
        return true;
    }

    virtual bool isTargetsMoreThanOne()
    {
        return false;
    }

    virtual bool targetIsMonster()
    {
        return true;
    }

    virtual int getPriority()
    {
        // TODO Auto-generated method stub
        return ActionSingleTarget::getPriority();
    }

    virtual bool update(long delta)
    {
        return false;
    }

    virtual void draw(Canvas *canvas)
    {
    }

};
