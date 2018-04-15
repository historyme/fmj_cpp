#include "ActionExecutor.h"

ActionExecutor::ActionExecutor(HMQueue<Action*> *actionQueue, Combat *combat)
    :mCurrentAction(NULL), mIsNewAction(true)
{
    mActionQueue = actionQueue;
    mCombat = combat;
}

void ActionExecutor::reset()
{
    if (mCurrentAction != NULL)
    {
        delete mCurrentAction;
    }
    mCurrentAction = NULL;
    mIsNewAction = true;
}

bool ActionExecutor::update(long delta)
{
    if (mCurrentAction == NULL)
    {
        mCurrentAction = mActionQueue->poll();
        if (mCurrentAction == NULL)
        {
            return false;
        }
        mCurrentAction->preproccess();
        mIsNewAction = false;
    }

    if (mIsNewAction)   // 跳过死亡角色
    {
        if (!fixAction())
        {
            return false;
        }
        mCurrentAction->preproccess();
        mIsNewAction = false;
    }

    if (!mCurrentAction->update(delta))   // 当前动作执行完毕
    {
        mCurrentAction->postExecute();

        delete mCurrentAction;
        mCurrentAction = NULL;

        mCurrentAction = mActionQueue->poll(); // 取下一个动作
        if (mCurrentAction == NULL)   // 所有动作执行完毕
        {
            return false;
        }
        mIsNewAction = true;
    }

    return true;
}

bool ActionExecutor::fixAction()
{
    // attacker dead, goto next action
    while (!mCurrentAction->isAttackerAlive())
    {
        mCurrentAction = mActionQueue->poll();
        if (mCurrentAction == NULL)
        {
            return false;
        }
    }

    // target dead, get an alive target
    if (!mCurrentAction->isTargetAlive())
    {
        if (mCurrentAction->isTargetsMoreThanOne())   // 敌人都死了
        {
            return false;
        }
        else     // try to find an alive target
        {
            FightingCharacter *newTarget = NULL;
            if (mCurrentAction->targetIsMonster())
            {
                newTarget = mCombat->getFirstAliveMonster();
            }
            else
            {
                newTarget = mCombat->getRandomAlivePlayer();
            }

            if (newTarget == NULL)
            {
                return false;
            }
            else if (!mCurrentAction->bInstanceof_ActionFlee)
            {
                ((ActionSingleTarget *)mCurrentAction)->setTarget(newTarget);
            }
        }
    }

    return true;
}

void ActionExecutor::draw(Canvas *canvas)
{
    if (mCurrentAction != NULL)
    {
        mCurrentAction->draw(canvas);
    }
}
