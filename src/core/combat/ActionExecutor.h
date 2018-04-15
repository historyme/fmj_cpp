#ifndef _ActionExecutor_h_
#define _ActionExecutor_h_

#include <iostream>
#include "HMQueue.h"

#include "Canvas.h"
#include "FightingCharacter.h"
#include "Action.h"
#include "ActionFlee.h"
#include "ActionSingleTarget.h"
#include "Combat.h"

using namespace std;

class ActionExecutor
{
private:
    /* 被执行的动作队列 */
    HMQueue<Action*> *mActionQueue;
    /* 当前执行的动作 */
    Action *mCurrentAction;
    bool mIsNewAction;
    Combat *mCombat;

public:
    ActionExecutor(HMQueue<Action*> *actionQueue, Combat *combat);

    void reset();

    /**
     *
     * @param delta
     * @return 执行完毕返回<code>false</code>，否则返回<code>true</code>
     */
    bool update(long delta);

    /**
     * 执行完毕返回<code>false</code>
     */
    bool fixAction();

    void draw(Canvas *canvas);

};

#endif
