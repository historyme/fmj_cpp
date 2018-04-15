#include "ActionPhysicalAttackAll.h"

ActionPhysicalAttackAll::ActionPhysicalAttackAll(FightingCharacter *attacker, vector <FightingCharacter *> targets) :
    ActionMultiTarget(attacker, targets)
{

}

ActionPhysicalAttackAll::~ActionPhysicalAttackAll()
{

}
