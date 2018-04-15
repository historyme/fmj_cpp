#ifndef _ActionSingleTarget_h_
#define _ActionSingleTarget_h_

#include <iostream>

#include "Canvas.h"
#include "FightingCharacter.h"
#include "Monster.h"
#include "RaiseAnimation.h"
#include "Action.h"


class ActionSingleTarget: public Action
{

protected:
    FightingCharacter *mTarget;

protected:
    RaiseAnimation *mRaiseAni;

public:
    ActionSingleTarget(FightingCharacter *attacker, FightingCharacter *target);
    virtual ~ActionSingleTarget();
    virtual void postExecute();

protected:
    virtual bool updateRaiseAnimation(long delta);
    virtual void drawRaiseAnimation(Canvas *canvas);

public:
    virtual bool isTargetAlive();
    virtual bool isTargetsMoreThanOne();
    virtual bool targetIsMonster();
    void setTarget(FightingCharacter *fc);
};

#endif
