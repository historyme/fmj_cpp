#include "ActionMultiTarget.h"

ActionMultiTarget::ActionMultiTarget(FightingCharacter *attacker, vector <FightingCharacter *> targets)
{
    mAttacker = attacker;

    mTargets.clear();
    std::vector<FightingCharacter *>::const_iterator iter;
    for (iter = targets.begin(); iter != targets.end(); ++iter)
    {
        FightingCharacter *i = *iter;
        mTargets.push_back(i);
    }

    mRaiseAnis.clear();
}

ActionMultiTarget::~ActionMultiTarget()
{
    while (!mRaiseAnis.empty())
    {
        delete mRaiseAnis.back();
        mRaiseAnis.pop_back();
    }
}
