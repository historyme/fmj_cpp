#pragma once
#include "ActionSingleTarget.h"

class  ActionMagicAttackOne : public ActionSingleTarget
{
private:
    int mState;
    BaseMagic *magic; // MagicAttack MagicSpecial
    ResSrs *mAni;
    int mAniX, mAniY;
    int ox, oy;

public:
    ActionMagicAttackOne(FightingCharacter *attacker, FightingCharacter *target, BaseMagic *magic);
    virtual ~ActionMagicAttackOne();

    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
};
