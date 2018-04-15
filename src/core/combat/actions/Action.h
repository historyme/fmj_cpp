#ifndef _Action_h_
#define _Action_h_

#include <iostream>
#include "FightingCharacter.h"
#include "Canvas.h"

class Action
{
public:
    Action();
    virtual ~Action() {}

    /**
    * 动作产生的影响，播放动作动画之前执行一次。
    */
    virtual void preproccess() {}

    /**
    * 隐藏死亡角色
    */
    virtual void postExecute() = 0;

protected:
    virtual bool updateRaiseAnimation(long delta) = 0;
    virtual void drawRaiseAnimation(Canvas *canvas) = 0;

public:
    /**
    *
    * @param delta
    * @return 执行完毕返回<code>false</code>，否则返回<code>true</code>
    */
    virtual bool update(long delta);

    virtual void draw(Canvas *canvas) {}

    /**
    *
    * @return 动作发起者的身法
    */
    virtual int getPriority();

    bool isAttackerAlive();

    virtual bool isTargetAlive() = 0;
    virtual bool isTargetsMoreThanOne() = 0;
    virtual bool targetIsMonster() = 0;

private:
    static const int DELTA = 1000 / 20;
    long mTimeCnt;

public:
    bool bInstanceof_ActionFlee;
    bool bInstanceof_ActionCoopMagic;

protected:
    int mCurrentFrame;

    /* 动作的发起者 */
    FightingCharacter *mAttacker;

public:
    enum
    {
        STATE_MOV = 0, // 移位动画
        STATE_PRE = 1, // 起手动画
        STATE_ANI = 2, // 魔法动画
        STATE_AFT = 3, // 伤害动画
        MOV_FRAME = 5, // 移位帧数
    };
};

#endif
