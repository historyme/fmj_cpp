#pragma once
#include "ActionMultiTarget.h"
class GoodsHiddenWeapon;

class  ActionThrowItemAll : public ActionMultiTarget
{
private:
    int mState;
    ResSrs *mAni;
    int ox, oy;
    GoodsHiddenWeapon *hiddenWeapon;

public:
    ActionThrowItemAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, GoodsHiddenWeapon *goods);
    virtual ~ActionThrowItemAll();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
};
