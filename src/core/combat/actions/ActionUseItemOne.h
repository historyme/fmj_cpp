#pragma once
#include "ActionSingleTarget.h"

class  ActionUseItemOne : public ActionSingleTarget
{
private:
    int mState;
    ResSrs *mAni;
    int mAnix, mAniy;
    int ox, oy;
    BaseGoods *goods;

public:
    ActionUseItemOne(FightingCharacter *attacker, FightingCharacter *target, BaseGoods *g);
    virtual ~ActionUseItemOne();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
};
