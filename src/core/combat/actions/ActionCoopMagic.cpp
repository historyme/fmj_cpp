#include "ActionCoopMagic.h"
#include "Player.h"
#include "FightingCharacter.h"

ActionCoopMagic::ActionCoopMagic(vector<FightingCharacter *> actors, FightingCharacter *monster)
{
    bInstanceof_ActionCoopMagic = true;
    mState = STATE_MOV;
    mActors = actors;
    mMonster = monster;
    onlyOneMonster = true;
    mRaiseAni = NULL;

    magic = getCoopMagic();
}

ActionCoopMagic::ActionCoopMagic(vector<FightingCharacter *> actors, vector<FightingCharacter *> monsters)
{
    bInstanceof_ActionCoopMagic = true;
    mState = STATE_MOV;
    mActors = actors;
    mMonsters = monsters;
    onlyOneMonster = FALSE;
    mRaiseAnis.clear();

    magic = getCoopMagic();
}

ActionCoopMagic::~ActionCoopMagic()
{
    delete dxy[0];
    delete dxy[1];
    delete oxy[0];
    delete oxy[1];

    if (mRaiseAni != NULL)
    {
        delete mRaiseAni;
    }

    while(!mRaiseAnis.empty())
    {
        delete mRaiseAnis.back();
        mRaiseAnis.pop_back();
    }
}

MagicAttack * ActionCoopMagic::getCoopMagic()
{
    Player *firstPlayer = (Player*)mActors.at(0);
    GoodsDecorations *dc = (GoodsDecorations*)((firstPlayer->getEquipmentsArray())[0]);
    return dc == NULL ? NULL : dc->getCoopMagic();
}

void ActionCoopMagic::preproccess()
{
    // TODO 记下伤害值、异常状态
    int midpos[3][2] = {{92, 52}, { 109, 63 }, { 126, 74 }};
    int size = mActors.size();

    dxy[0] = new float[size];
    dxy[1] = new float[size];

    oxy[0] = new int[size];
    oxy[1] = new int[size];

    for (int i = 0; i < size; i++)
    {
        oxy[i][0] = mActors.at(i)->getCombatX();
        oxy[i][1] = mActors.at(i)->getCombatY();
    }
    for (int i = 0; i < size; i++)
    {
        dxy[i][0] = midpos[i][0] - oxy[i][0];
        dxy[i][0] /= MOV_FRAME;
        dxy[i][1] = midpos[i][1] - oxy[i][1];
        dxy[i][1] /= MOV_FRAME;
    }

    if (onlyOneMonster)
    {
        mAniX = mMonster->getCombatX();
        mAniY = mMonster->getCombatY() - mMonster->getFightingSprite()->getHeight() / 2;
    }
    else
    {
        mAniX = mAniY = 0;
    }

    if (magic == NULL)
    {
        mAni = (ResSrs*)DatLib::GetRes(DatLib::RES_SRS, 2, 240);
    }
    else
    {
        mAni = magic->getMagicAni();
    }
    mAni->startAni();

    //TODO 计算扣血数字，画出数字
}

bool ActionCoopMagic::update(long delta)
{
    Action::update(delta);
    // TODO Auto-generated method stub
    switch (mState)
    {
    case STATE_MOV:
        if (mCurrentFrame < MOV_FRAME)
        {
            for (int i = 0; i < mActors.size(); i++)
            {
                mActors.at(i)->setCombatPos((int)(oxy[i][0] + dxy[i][0] * mCurrentFrame),
                    (int)(oxy[i][1] + dxy[i][1] * mCurrentFrame));
            }
        }
        else
        {
            mState = STATE_PRE;
        }
        break;

    case STATE_PRE:
        if (mCurrentFrame < 10 + MOV_FRAME)
        {
            for (int i = 0; i < mActors.size(); i++)
            {
                mActors.at(i)->getFightingSprite()->setCurrentFrame(
                    (mCurrentFrame - MOV_FRAME) * 3 / 10 + 6);
            }
        }
        else
        {
            mState = STATE_ANI;
        }
        break;

    case STATE_ANI:
        if (!mAni->update(delta))
        {
            mState = STATE_AFT;
            for (int i = 0; i < mActors.size(); i++)
            {
                static_cast<Player*>(mActors.at(i))->setFrameByState();
                mActors.at(i)->setCombatPos(oxy[i][0], oxy[i][1]);
            }
        }
        break;

    case STATE_AFT:
        if (onlyOneMonster)
        {
            //              return m
        }
        if (true) return false;
        break;
    }
    return true;
}

void ActionCoopMagic::draw(Canvas *canvas)
{
    if (mState == STATE_ANI)
    {
        mAni->drawAbsolutely(canvas, mAniX, mAniY);
    }
}

int ActionCoopMagic::getPriority()
{
    return mActors.at(0)->getSpeed();
}

bool ActionCoopMagic::isAttackerAlive()
{
    return mActors.at(0)->isAlive();
}

void ActionCoopMagic::postExecute()
{
    // TODO Auto-generated method stub
}

bool ActionCoopMagic::updateRaiseAnimation(long delta)
{
    if (onlyOneMonster)
    {
        return mRaiseAni != NULL && mRaiseAni->update(delta);
    }

    if (!mRaiseAnis.empty())
    {
        // 全体
        if (mRaiseAnis.size() <= 0)
        {
            return false;
        }
        else
        {
            for (int i = 0; i < mRaiseAnis.size(); i++)
            {
                if (!mRaiseAnis.at(0)->update(delta))
                {
                    delete mRaiseAnis.back();
                    mRaiseAnis.pop_back();

                    if (mRaiseAnis.empty())
                        return false;
                }
            }
            return true;
        }
    }

    return false;
}

void ActionCoopMagic::drawRaiseAnimation(Canvas *canvas)
{
    if (onlyOneMonster)
    {
        if (mRaiseAni != NULL)
        {
            mRaiseAni->draw(canvas);
        }
    }
    else
    {
        if (!mRaiseAnis.empty())
        {
            vector<RaiseAnimation*>::const_iterator iter = mRaiseAnis.begin();
            for (; iter != mRaiseAnis.end(); ++iter)
            {
                RaiseAnimation *ani = *iter;
                ani->draw(canvas);
            }
        }
    }
}

bool ActionCoopMagic::isTargetAlive()
{
    if (onlyOneMonster)
    {
        return mMonster->isAlive();
    }
    else
    {
        vector<FightingCharacter*>::const_iterator iter = mMonsters.begin();
        for (; iter != mMonsters.end(); ++iter)
        {
            FightingCharacter *m = *iter;
            if (m->isAlive())
            {
                return true;
            }
        }
    }
    return FALSE;
}

