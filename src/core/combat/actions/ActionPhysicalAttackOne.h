#pragma once

#include "ActionSingleTarget.h"
#include "Random.h"

class FightingCharacter;

class  ActionPhysicalAttackOne : public ActionSingleTarget
{
private:
    static const int TOTAL_FRAME = 5;
    float dx, dy;
    int ox, oy;
    int buffRound;
    bool mTotalMark;
    Random random;

public:
    ActionPhysicalAttackOne(FightingCharacter *attacker, FightingCharacter *target);
    virtual ~ActionPhysicalAttackOne();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
    virtual int getPriority();
};
