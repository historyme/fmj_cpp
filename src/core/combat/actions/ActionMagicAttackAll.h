#pragma once

#include "ActionMultiTarget.h"

class MagicAttack;
class ResSrs;

using namespace std;

class  ActionMagicAttackAll : public ActionMultiTarget
{
private:
    int mState;
    MagicAttack *magic; // MagicAttack
    ResSrs *mAni;
    int ox, oy;

public:
    ActionMagicAttackAll(FightingCharacter *attacker, vector <FightingCharacter *> targets, MagicAttack *magic);
    virtual ~ActionMagicAttackAll();

    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
    virtual int getPriority();
};
