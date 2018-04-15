#pragma once
#include "ActionSingleTarget.h"
class BaseMagic;
class FightingCharacter;
class ResSrs;
using namespace std;

class  ActionMagicHelpOne : public ActionSingleTarget
{
private:
    int mState;
    BaseMagic *magic;
    ResSrs *mAni;
    int mAnix, mAniy;
    int ox, oy;

public:
    ActionMagicHelpOne(FightingCharacter *attacker, FightingCharacter *target, BaseMagic *magic);
    virtual ~ActionMagicHelpOne();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
};
