#pragma once
#include "ActionMultiTarget.h"

class  ActionUseItemAll : public ActionMultiTarget
{
private:
    int mState;
    BaseGoods *goods;
    ResSrs *mAni;
    int ox, oy;

public:
    ActionUseItemAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, BaseGoods *goods);
    ~ActionUseItemAll();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
    virtual int getPriority();
};
