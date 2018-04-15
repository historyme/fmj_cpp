#ifndef _ActionMultiTarget_H_
#define _ActionMultiTarget_H_

#include "Action.h"
#include "RaiseAnimation.h"
#include "FightingCharacter.h"

using namespace std;

class  ActionMultiTarget : public Action
{
protected:
    vector<FightingCharacter*> mTargets;
    vector<RaiseAnimation*> mRaiseAnis;

public:
    ActionMultiTarget(FightingCharacter *attacker, vector <FightingCharacter *> targets);
    virtual ~ActionMultiTarget();

    void postExecute()
    {
        if (mTargets.size() > 0)
        {
            std::vector<FightingCharacter *>::const_iterator iter;
            for (iter = mTargets.begin(); iter != mTargets.end(); ++iter)
            {
                FightingCharacter *i = *iter;
                i->setVisiable(i->isAlive());
            }
        }
    }

protected:
    virtual bool updateRaiseAnimation(long delta)
    {
        if (mRaiseAnis.size() >= 0)
        {
            if (mRaiseAnis.size() == 0)
            {
                return false;
            }
            else
            {
                for (int i = 0; i < mRaiseAnis.size(); i++)
                {
                    if (!mRaiseAnis.at(i)->update(delta))
                    {
                        vector<RaiseAnimation *>::iterator iter = mRaiseAnis.begin();
                        mRaiseAnis.erase(iter + i);
                        if (mRaiseAnis.empty()) return false;
                    }
                }
                return true;
            }
        }

        return false;
    }

    virtual void drawRaiseAnimation(Canvas *canvas)
    {
        if (mRaiseAnis.size() > 0)
        {
            std::vector<RaiseAnimation *>::const_iterator iter;
            for (iter = mRaiseAnis.begin(); iter != mRaiseAnis.end(); ++iter)
            {
                RaiseAnimation *i = *iter;
                i->draw(canvas);
            }
        }
    }

public:
    virtual void draw(Canvas *canvas)
    {
        // TODO Auto-generated method stub

    }

    virtual bool isTargetAlive()
    {
        std::vector<FightingCharacter *>::const_iterator iter;
        for (iter = mTargets.begin(); iter != mTargets.end(); ++iter)
        {
            FightingCharacter *i = *iter;
            if (i->isAlive())
            {
                return true;
            }
        }

        return false;
    }

    virtual bool isTargetsMoreThanOne()
    {
        // TODO Auto-generated method stub
        return false;
    }

    virtual bool targetIsMonster()
    {
        return mTargets.at(0)->bInstanceof_Monster;
    }

};

#endif
