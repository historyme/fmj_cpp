#pragma once

#include "Action.h"
#include "Monster.h"
#include "MagicAttack.h"
#include "RaiseAnimation.h"
#include "GoodsDecorations.h"
#include <iostream>

using namespace std;

class  ActionCoopMagic : public Action
{
private:
    int mState;
    vector<FightingCharacter *> mActors;
    vector<FightingCharacter *> mMonsters;
    FightingCharacter *mMonster;
    bool onlyOneMonster;
    MagicAttack *magic;
    ResSrs *mAni;
    RaiseAnimation *mRaiseAni;
    vector<RaiseAnimation *> mRaiseAnis;

private:
    float *dxy[2];
    int *oxy[2];
    int mAniX, mAniY;

public:
    ActionCoopMagic(vector<FightingCharacter *> actors, FightingCharacter *monster);
    ActionCoopMagic(vector<FightingCharacter *> actors, vector<FightingCharacter *> monsters);
    virtual ~ActionCoopMagic();

private:
    MagicAttack *getCoopMagic();

public:
    virtual void preproccess();
    virtual bool update(long delta);
    virtual void draw(Canvas *canvas);
    virtual int getPriority();
    virtual bool isAttackerAlive();
    virtual void postExecute();
    virtual bool updateRaiseAnimation(long delta);
    virtual void drawRaiseAnimation(Canvas *canvas);
    virtual bool isTargetAlive();
    virtual bool isTargetsMoreThanOne() {return onlyOneMonster;}
    virtual bool targetIsMonster() { return true; }
};
