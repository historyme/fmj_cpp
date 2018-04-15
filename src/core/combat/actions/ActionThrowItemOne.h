#pragma once
#include "ActionSingleTarget.h"

class GoodsHiddenWeapon;

class  ActionThrowItemOne : public ActionSingleTarget
{
private:
    int mState;
    ResSrs *mAni;
    int mAniX, mAniY;
    int ox, oy;
    GoodsHiddenWeapon *hiddenWeapon;

public:
    ActionThrowItemOne(FightingCharacter *attacker, FightingCharacter *target, GoodsHiddenWeapon *g);
    virtual ~ActionThrowItemOne();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
};
