#pragma once
#include "ActionMultiTarget.h"
class BaseMagic;
class FightingCharacter;
class ResSrs;
using namespace std;

class  ActionMagicHelpAll : public ActionMultiTarget
{
private:
    int mState;
    BaseMagic *magic;
    ResSrs *mAni;
    int ox, oy;

public:
    ActionMagicHelpAll(FightingCharacter *attacker,
        vector<FightingCharacter *> targets, BaseMagic *magic);
    virtual ~ActionMagicHelpAll();
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
    virtual int getPriority();
};
