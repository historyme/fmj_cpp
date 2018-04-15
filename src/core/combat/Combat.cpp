#include "Combat.h"
#include "Bitmap.h"
#include "ScreenMainGame.h"
#include "ActionExecutor.h"
#include "CombatUI.h"
#include "CombatSuccess.h"
#include "ResLevelupChain.h"
#include "ActionPhysicalAttackAll.h"
#include "ActionPhysicalAttackOne.h"
#include "SaveLoadStream.h"


Combat::Combat()
    :mIsEnable(false), mIsFighting(false), mIsRandomFight(false)
{
    mRandom = new Random();

    mIsWin = false;

    mPlayerPos[0].set(64 + 12, 52 + 18);
    mPlayerPos[1].set(96 + 12, 48 + 18);
    mPlayerPos[2].set(128 + 12, 40 + 18);

    mIsAutoAttack = false;
    mFlyPeach = (ResSrs*)DatLib::GetRes(DatLib::RES_SRS, 1, 249);
    mCombatState = SelectAction;
    mActionQueue = new HMQueue < Action * > ;
    mActionExecutor = new ActionExecutor(mActionQueue, this);
    mCombatUI = CombatUI::getInstance();
    mCombatUI->setCurrentPlayerIndex(0);
    mCombatUI->setCallBack(this);
    mCombatSuccess = NULL;
    mCurSelActionPlayerIndex = 0;
    mTimeCnt = 0;
    mBackground = NULL;
}

Combat::~Combat()
{
    delete mFlyPeach;
    delete mActionExecutor;
    delete mActionQueue;
    delete mRandom;
}

Combat * Combat::getInstance()
{
    static Combat instance;
    return &instance;
}

bool Combat::isActive()
{
    return mIsEnable && mIsFighting;
}

void Combat::fightEnable()
{
    mIsEnable = true;
}

void Combat::fightDisable()
{
    mIsEnable = false;
}

void Combat::initFight(vector<int> monstersType, int scrb, int scrl, int scrr)
{
    mIsEnable = true;
    mIsRandomFight = true;
    mIsFighting = false;

    mMonsterType.clear();

    for (int i = 0; i < (int)(monstersType.size()); ++i)
    {
        if (monstersType[i] > 0)
        {
            mMonsterType.push_back(monstersType.at(i));
        }
    }
    mRoundCnt = 0;
    mMaxRound = 0; // 回合数无限制
    createBackgroundBitmap(scrb, scrl, scrr);
}

void Combat::enterFight(int roundMax, int monstersType[3], int scr[3], int evtRnds[3], int evts[3], int lossto, int winto)
{
    mIsRandomFight = false;

    for (int i = 0; i < 3; ++i)
    {
        if (monstersType[i] > 0)
        {
            Monster *tmp = (Monster*)DatLib::GetRes(DatLib::RES_ARS, 3, monstersType[i]);
            if (tmp != NULL)
            {
                mMonsterList.push_back(tmp);
            }
        }
    }
    mMaxRound = roundMax;
    mRoundCnt = 0;
    prepareForNewCombat();
    createBackgroundBitmap(scr[0], scr[1], scr[2]);

    mEventRound.clear();
    mEventNum.clear();
    for (int i = 0; i < 3; i++)
    {
        mEventRound.push_back(evtRnds[i]);
        mEventNum.push_back(evts[i]);
    }
    mLossAddr = lossto;
    mWinAddr = winto;
}

bool Combat::startNewRandomCombat()
{
    if (!mIsEnable || mRandom->nextInt(COMBAT_PROBABILITY) != 0)
    {
        mIsFighting = false;
        return false;
    }
    // 打乱怪物类型
    for (int i = mMonsterType.size() - 1; i > 1; --i)
    {
        int r = mRandom->nextInt(i);
        int t = mMonsterType[i];
        mMonsterType[i] = mMonsterType[r];
        mMonsterType[r] = t;
    }

    //删除怪物列表
    while (!mMonsterList.empty())
    {
        delete mMonsterList.back();
        mMonsterList.pop_back();
    }
    // 随机添加怪物
    for (int i = mRandom->nextInt(3), j = 0; i >= 0; i--)
    {
        Monster *m = (Monster*)DatLib::GetRes(DatLib::RES_ARS, 3, mMonsterType.at(j++));
        if (m != NULL)
        {
            mMonsterList.push_back(m);
        }
    }
    mRoundCnt = 0;
    mMaxRound = 0; // 回合不限
    prepareForNewCombat();
    return true;
}

void Combat::createBackgroundBitmap(int scrb, int scrl, int scrr)
{
    if (NULL != mBackground)
    {
        delete mBackground;
    }
    mBackground = Bitmap::createBitmap(160, 96, ARGB_8888);
    Canvas *canvas = new Canvas(mBackground);
    
    ResImage *img;
    
    img = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 4, scrb);
    if (img != NULL)
    {
        img->draw(canvas, 1, 0, 0); // 背景
        delete img;
    }

    img = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 4, scrl);
    if (img != NULL)
    {
        img->draw(canvas, 1, 0, 96 - img->getHeight()); // 左下角
        delete img;
    }

    img = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 4, scrr);
    if (img != NULL)
    {
        img->draw(canvas, 1, 160 - img->getWidth(), 0); // 右上角
        delete img;
    }

    delete canvas;

    mScrb = scrb;
    mScrl = scrl;
    mScrR = scrr;
}

void Combat::prepareForNewCombat()
{
    mIsEnable = true;
    mIsFighting = true;

    //删动作物列表
    while (!mActionQueue->empty())
    {
        delete mActionQueue->back();
        mActionQueue->pop_back();
    }

    mIsAutoAttack = false;
    mCombatState = SelectAction;
    mCurSelActionPlayerIndex = 0;

    mPlayerList.clear();
    for (int i = 0; i < (int)(ScreenMainGame::sPlayerList.size());i++)
    {
        mPlayerList.push_back(ScreenMainGame::sPlayerList.at(i));
    }

    mCombatUI->reset();
    mCombatUI->setCurrentPlayerIndex(0);
    mCombatUI->setPlayerList(mPlayerList);
    setOriginalPlayerPos();
    setOriginalMonsterPos();
    mRoundCnt = 0;
    mHasEventExed = false;
    // 检查玩家血量
    for (int i = 0; i < (int)(mPlayerList.size()); ++i)
    {
        Player *p = mPlayerList.at(i);
        if (p->getHP() <= 0)
        {
            // 确保血量大于0
            p->setHP(1);
        }
        p->setFrameByState();
    }

    // 怪物血量设置为其最大值
    for (int i = 0; i < (int)(mMonsterList.size()); ++i)
    {
        Monster *m = mMonsterList.at(i);
        m->setHP(m->getMaxHP());
    }

    // 计算战斗胜利能获得的金钱和经验
    mWinMoney = 0;
    mWinExp = 0;
    for (int i = 0; i < (int)(mMonsterList.size()); ++i)
    {
        Monster *m = mMonsterList.at(i);
        mWinMoney += m->getMoney();
        mWinExp += m->getExp();
    }

    if (!mIsRandomFight && mMonsterList.size() == 1)
    {
        // 剧情战斗，只有一个怪时，怪的位置在中间
        Monster *m = mMonsterList.at(0);
        Monster *n = (Monster*)DatLib::GetRes(DatLib::RES_ARS, m->getType(), m->getIndex());
        n->setHP(-1);
        n->setVisiable(false);

        vector<Monster *>::iterator iter = mMonsterList.begin();
        mMonsterList.insert(iter, n); // 加入一个看不见的怪 ,在0位置

        setOriginalMonsterPos(); // 重置位置
    }
    mFlyPeach->startAni();
    mFlyPeach->setIteratorNum(5);

    mCombatUI->setMonsterList(mMonsterList);
}

void Combat::exitCurrentCombat()
{
    if (!mIsRandomFight)
    {
        ScreenMainGame::gotoAddress(mIsWin ? mWinAddr : mLossAddr);
        ScriptExecutor::goonExecute = true;
        mIsRandomFight = true;
    }
    else
    {
        if (!mIsWin)
        {
            // 死了，游戏结束
            GameView::getInstance()->changeScreen(SCREEN_MENU);
        }
    }
    mIsFighting = false;

    while (!mActionQueue->empty())
    {       
        Action *act = mActionQueue->back();
        if (NULL != act)
        {
            delete act;
        }
        mActionQueue->pop_back();
    }
    
    mActionExecutor->reset();
    mCombatUI->reset();
    mIsAutoAttack = false;
    // 恢复一定的血量
    vector<Player*>::const_iterator iter = mPlayerList.begin();
    for (; iter != mPlayerList.begin(); ++iter)
    {
        Player *p = *iter;

        if (p->getHP() <= 0)
        {
            p->setHP(1);
        }
        if (p->getMP() <= 0)
        {
            p->setMP(1);
        }
        p->setHP(p->getHP() + (p->getMaxHP() - p->getHP()) / 10);
        p->setMP(p->getMP() + p->getMaxMP() / 5);
        if (p->getMP() > p->getMaxMP())
        {
            p->setMP(p->getMaxMP());
        }
   }
}

void Combat::setOriginalPlayerPos()
{
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        mPlayerList.at(i)->setCombatPos(mPlayerPos[i].x, mPlayerPos[i].y);
    }
}

void Combat::setOriginalMonsterPos()
{
    for (int i = 0; i < (int)(mMonsterList.size()); i++)
    {
        mMonsterList.at(i)->setOriginalCombatPos(i);
    }
}

void Combat::generateAutoActionQueue()
{
    Monster *monster = getFirstAliveMonster();

    while (!mActionQueue->empty())
    {
        Action *act = mActionQueue->back();
        if (NULL != act)
        {
            delete act;
        }
        mActionQueue->pop_back();
    }

    // 玩家的Action
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        Player *p = mPlayerList.at(i);
        if (p->isAlive())
        {
            Action *act = NULL;
            if (p->hasAtbuff(Player::BUFF_MASK_ALL))
            {
                vector<FightingCharacter *> fc;
                fc.clear();
                while (!mMonsterList.empty())
                {
                    fc.push_back(mMonsterList.back());
                }

                act = new ActionPhysicalAttackAll(p, fc);
            }
            else
            {
                act = new ActionPhysicalAttackOne(p, monster);
            }
            mActionQueue->add(act);
        }
    }
    // 怪物的Action
    generateMonstersActions();
    sortActionQueue();
}

void Combat::generateMonstersActions()
{
    // TODO according to the monster's intelligence, add some magic attack
    for (int i = 0; i < (int)(mMonsterList.size()); i++)
    {
        Monster *m = mMonsterList.at(i);
        if (m->isAlive())
        {
            Player *p = getRandomAlivePlayer();
            if (p != NULL)
            {
                Action *act = NULL;
                if (p->hasAtbuff(Monster::BUFF_MASK_ALL))
                {
                    vector<FightingCharacter *> fc;
                    fc.clear();
                    while (!mPlayerList.empty())
                    {
                        fc.push_back(mPlayerList.back());
                    }

                    act = new ActionPhysicalAttackAll(m, fc);
                }
                else
                {
                    act = new ActionPhysicalAttackOne(m, p);
                }
                mActionQueue->add(act);
            }
        }
    }
}

bool Combat::isAnyPlayerAlive()
{
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        Player *p = mPlayerList.at(i);
        if (p->getHP() > 0)
        {
            return true;
        }
    }
    return false;
}

bool Combat::isAllMonsterDead()
{
    return getFirstAliveMonster() == NULL;
}

bool Combat::isPlayerBehindDead(int index)
{
    for (int i = index + 1; i < (int)(mPlayerList.size()); i++)
    {
        if (mPlayerList.at(i)->isAlive())
        {
            return false;
        }
    }
    return true;
}

int Combat::getNextAlivePlayerIndex()
{
    for (int i = mCurSelActionPlayerIndex + 1; i < (int)(mPlayerList.size()); i++)
    {
        if (mPlayerList.at(i)->isAlive())
        {
            return i;
        }
    }
    return -1;
}

int Combat::getPreAlivePlayerIndex()
{
    for (int i = mCurSelActionPlayerIndex - 1; i >= 0; i--)
    {
        if (mPlayerList.at(i)->isAlive())
        {
            return i;
        }
    }
    return -1;
}

int Combat::getFirstAlivePlayerIndex()
{
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        if (mPlayerList.at(i)->isAlive())
        {
            return i;
        }
    }
    return -1;
}

void Combat::update(long delta)
{
    mTimeCnt += delta;
    switch (mCombatState)
    {
      case SelectAction:
          if (!mHasEventExed && !mIsRandomFight)
          {
              mHasEventExed = true;
              for (int i = 0; i < (int)(mEventRound.size()); i++)
              {
                  if (mRoundCnt == mEventRound[i] && mEventNum[i] != 0)
                  {
                      ScreenMainGame::triggerEvent(mEventNum[i]);
                  }
              }
          }
          if (mIsAutoAttack)
          {
              // 自动生成动作队列
              generateAutoActionQueue();
              mCombatState = PerformAction;
          }
          else
          {
              // 玩家决策
              mCombatUI->update(delta);
          }
          break;
      case PerformAction:
          if (!mActionExecutor->update(delta))
          {
              // 动作执行完毕
              if (isAllMonsterDead())
              {
                  // 怪物全挂
                  mTimeCnt = 0; // 计时器清零
                  mCombatState = Win;
                  Player::sMoney += mWinMoney; // 获得金钱

                  vector<Player*> lvuplist;
                  lvuplist.clear();

                  for (int i = 0; i < (int)(mPlayerList.size()); i++)
                  {
                      Player *p = mPlayerList.at(i);

                      // 获得经验
                      if (p->isAlive())
                      {
                          if (p->getLevel() >= p->getLevelupChain()->getMaxLevel()) // 满级
                              break;
                          int nextExp = p->getLevelupChain()->getNextLevelExp(p->getLevel());
                          int exp = mWinExp + p->getCurrentExp();
                          if (exp < nextExp)
                          {
                              p->setCurrentExp(exp);
                          }
                          else
                          {
                              // 升级
                              int cl = p->getLevel(); // 当前等级
                              ResLevelupChain *c = p->getLevelupChain();
                              p->setCurrentExp(exp - nextExp);
                              p->setLevel(cl + 1);
                              p->setMaxHP(p->getMaxHP() + c->getMaxHP(cl + 1) - c->getMaxHP(cl));
                              //p->setHP(p->getMaxHP()); CombatSuccess 中设置
                              p->setMaxMP(p->getMaxMP() + c->getMaxMP(cl + 1) - c->getMaxMP(cl));
                              //p->setMP(p->getMaxMP()); CombatSuccess 中设置
                              p->setAttack(p->getAttack() + c->getAttack(cl + 1) - c->getAttack(cl));
                              p->setDefend(p->getDefend() + c->getDefend(cl + 1) - c->getDefend(cl));
                              p->getMagicChain()->setLearnNum(c->getLearnMagicNum(cl + 1));
                              p->setSpeed(p->getSpeed() + c->getSpeed(cl + 1) - c->getSpeed(cl));
                              p->setLingli(p->getLingli() + c->getLingli(cl + 1) - c->getLingli(cl));
                              p->setLuck(p->getLuck() + c->getLuck(cl + 1) - c->getLuck(cl));
                              lvuplist.push_back(p);
                          }
                      }
                  }
                  // 最大幸运值
                  int ppt = 10;
                  for (int i = 0; i < (int)(mPlayerList.size()); i++)
                  {
                      Player *p = mPlayerList.at(i);
                      if (p->getLuck() > ppt)
                      {
                          ppt = p->getLuck();
                      }
                  }
                  ppt -= 10;
                  if (ppt > 100)
                  {
                      ppt = 100;
                  }
                  else if (ppt < 0)
                  {
                      ppt = 10;
                  }
                  // 战利品链表
                  GoodsManage gm;
                  vector<BaseGoods*> gl;

                  for (int i = 0; i < (int)(mMonsterList.size()); i++)
                  {
                      Monster *m = mMonsterList.at(i);
                      BaseGoods *g = m->getDropGoods();
                      int lucky = mRandom->nextInt(101);

                      if (g != NULL &&  lucky < ppt)
                      {
                          //  ppt%掉率
                          gm.addGoods(g->getType(), g->getIndex(), g->getGoodsNum());
                          Player::sGoodsList->addGoods(g->getType(), g->getIndex(), g->getGoodsNum()); // 添加到物品链表
                      }

                      if (g != NULL)
                      {
                          delete g;
                      }
                  }

                  gl.clear();
                  if (!gm.getGoodsList()->empty())
                  {
                      vector<BaseGoods *>::const_iterator iter = gm.getGoodsList()->begin();
                      for (; iter != gm.getGoodsList()->end(); ++iter)
                      {
                          BaseGoods *g = *iter;
                          gl.push_back(g);
                      }
                  }
                  if (!gm.getEquipList()->empty())
                  {
                      vector<BaseGoods *>::const_iterator iter = gm.getEquipList()->begin();
                      for (; iter != gm.getEquipList()->end(); ++iter)
                      {
                          BaseGoods *g = *iter;
                          gl.push_back(g);
                      }           
                  }

                  mCombatSuccess = new CombatSuccess(mWinExp, mWinMoney, gl, lvuplist); // 显示玩家的收获

                  //删除怪物列表
                  while (!mMonsterList.empty())
                  {
                      delete mMonsterList.back();
                      mMonsterList.pop_back();
                  }
              }
              else
              {
                  // 还有怪物存活
                  if (isAnyPlayerAlive())
                  {
                      // 有玩家角色没挂，继续打怪
                      ++mRoundCnt;
                      updateFighterState();
                      mCombatState = SelectAction;
                      mCurSelActionPlayerIndex = getFirstAlivePlayerIndex();
                      mCombatUI->setCurrentPlayerIndex(mCurSelActionPlayerIndex);
                      for (int i = 0; i < (int)(mPlayerList.size()); i++)
                      {
                          Player *p = mPlayerList.at(i);
                          p->setFrameByState();
                      }
                  }
                  else
                  {
                      // 玩家角色全挂，战斗失败
                      mTimeCnt = 0;
                      mCombatState = Loss;
                  }
              }
          }
          break;
      case Win:
          // TODO if (winAddr...)
          //          if (mTimeCnt > 1000){
          //              mCombatState = CombatState.Exit;
          //          }
          mIsWin = true;
          if (mCombatSuccess->update(delta))
          {
              delete mCombatSuccess;
              mCombatSuccess = NULL;

              mCombatState = Exit;
          }
          break;
      case Loss:
          // TODO if (lossAddr...)
          if (mIsRandomFight && mFlyPeach->update(delta))
          {
          }
          else
          {
              mIsWin = false;
              mCombatState = Exit;
          }
          break;
      case Exit:
          exitCurrentCombat();
          break;
      }
}


void Combat::draw(Canvas *canvas)
{
    canvas->drawBitmap(mBackground, 0, 0);
    // draw the monsters and players
    for (int i = 0; i < (int)(mMonsterList.size()); i++)
    {
        Monster *fc = mMonsterList.at(i);
        if (fc->isVisiable())
        {
            fc->getFightingSprite()->draw(canvas);
        }
    }
    for (int i = mPlayerList.size() - 1; i >= 0; i--)
    {
        FightingSprite *f = mPlayerList.at(i)->getFightingSprite();
        f->draw(canvas);
    }
    if (mCombatState == SelectAction && !mIsAutoAttack)
    {
        mCombatUI->draw(canvas);
    }
    else if (mCombatState == PerformAction)
    {
        mActionExecutor->draw(canvas);
    }
    else if (mCombatState == Win)
    {
        TextRender::drawText(canvas, "Win", 20, 40);
        mCombatSuccess->draw(canvas);
    }
    else if (mCombatState == Loss && mIsRandomFight)
    {
        TextRender::drawText(canvas, "Loss", 20, 40);
        mFlyPeach->draw(canvas, 0, 0);
    }
}

void Combat::onKeyDown(int key)
{
    if (mCombatState == SelectAction)
    {
        if (!mIsAutoAttack)
        {
            mCombatUI->onKeyDown(key);
        }
    }
    else if (mCombatState == Win)
    {
        mCombatSuccess->onKeyDown(key);
    }
}

void Combat::onKeyUp(int key)
{
   if (mCombatState == SelectAction)
    {
        if (!mIsAutoAttack)
        {
            mCombatUI->onKeyUp(key);
        }
    }
    else if (mCombatState == Win)
    {
        mCombatSuccess->onKeyUp(key);
    }
    if (mIsAutoAttack && key == KEY_CANCEL)
    {
        // 退出“围攻”模式
        mIsAutoAttack = false;
    }
}

void Combat::sortActionQueue()
{
    struct ActionQueueCompare
    {
        bool operator()(Action *lhs, Action *rhs)
        {
            return rhs->getPriority() < lhs->getPriority();
        }
    }comp;

    sort(mActionQueue->begin(), mActionQueue->end(), comp);
}

Monster * Combat::getFirstAliveMonster()
{
    for (int i = 0; i < (int)(mMonsterList.size()); i++)
    {
        Monster *m = mMonsterList.at(i);
        if (m->isAlive())
        {
            return m;
        }
    }
    return NULL;
}

Player * Combat::getRandomAlivePlayer()
{
    int cnt = 0;
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        Player *p = mPlayerList.at(i);
        if (p->isAlive())
        {
            ++cnt;
        }
    }

    if (cnt == 0) 
        return NULL; // 全死了

    vector<Player*> arr;
    for (int i = 0; i < (int)(mPlayerList.size()); i++)
    {
        Player *p = mPlayerList.at(i);
        if (p->isAlive())
        {
            arr.push_back(p);
        }
    }

    return arr.at(mRandom->nextInt(cnt));
}

void Combat::updateFighterState()
{
    // TODO decrease the buff's round count
}

void Combat::onActionSelected(Action *action)
{
    mActionQueue->add(action);
    mCombatUI->reset(); // 重置战斗UI
    if (action->bInstanceof_ActionCoopMagic)
    {
        // 只保留合击
        while (!mActionQueue->empty())
        {
            Action *act = mActionQueue->back();
            if (NULL != act)
            {
                delete act;
            }
            mActionQueue->pop_back();
        }

        mActionQueue->add(action);
        generateMonstersActions();
        sortActionQueue();
        mCombatState = PerformAction;
    }
    else if (mCurSelActionPlayerIndex >= (int)(mPlayerList.size()) - 1 ||
        isPlayerBehindDead(mCurSelActionPlayerIndex))
    {
        // 全部玩家角色的动作选择完成
        generateMonstersActions();
        sortActionQueue();
        mCombatState = PerformAction; // 开始执行动作队列
    }
    else
    {
        // 选择下一个玩家角色的动作
        mCurSelActionPlayerIndex = getNextAlivePlayerIndex();
        //if (mPlayerList.get(mCurSelActionPlayerIndex).hasDebuff(0)) TODO 乱眠死不能自己选择action
        mCombatUI->setCurrentPlayerIndex(mCurSelActionPlayerIndex);
    }
}

void Combat::onAutoAttack()
{
    // clear all the actions that has been selected, enter into auto fight mode
    mCombatUI->reset();

    while (!mActionQueue->empty())
    {
        Action *act = mActionQueue->back();
        if (NULL != act)
        {
            delete act;
        }
        mActionQueue->pop_back();
    }

    mIsAutoAttack = true;
    mCombatState = SelectAction;
}

void Combat::onFlee()
{
   // TODO add flee action to all the other actor
    mCombatUI->reset(); // 重置战斗UI
    for (int i = mCurSelActionPlayerIndex; i < (int)(mPlayerList.size()); i++)
    {
        bool bFlee = mRandom->nextbool();
        if (mPlayerList.at(i)->isAlive() && bFlee && mIsRandomFight)
        {
            class RunnableFlee : public Runnable
            {
            private:
                Combat *_this;
            public:
                RunnableFlee(Combat *_this)
                {
                    this->_this = _this;
                }
                virtual void run()
                {
                    // 逃跑成功后执行
                    _this->mIsWin = true;
                    _this->mCombatState = Exit;
                }
            };

            // 50% 逃走
            mActionQueue->add(new ActionFlee(mPlayerList.at(i), true, new RunnableFlee(this)));
            break;
        }
        else
        {
            // 逃跑失败
            mActionQueue->add(new ActionFlee(mPlayerList.at(i), false, NULL));
        }
    }
    generateMonstersActions();
    sortActionQueue();
    mCombatState = PerformAction;
}

void Combat::onCancel()
{
    int i = getPreAlivePlayerIndex();
    if (i >= 0)
    {
        // 不是第一个角色
        // 重选上一个角色的动作
        mActionQueue->removeLast();
        mCurSelActionPlayerIndex = i;
        mCombatUI->setCurrentPlayerIndex(mCurSelActionPlayerIndex);
        mCombatUI->reset();
    }
}

void Combat::read(SaveLoadStream *buff)
{
    mIsEnable = buff->readBoolean();
    if (mIsEnable)
    {
        vector<int> monsterType;
        monsterType.clear();
        int size = buff->readInt();
        for (int i = 0; i < size; i++)
        {
            monsterType.push_back(buff->readInt());
        }

        int scrb = buff->readInt();
        int scrl = buff->readInt();
        int scrr = buff->readInt();
        initFight(monsterType, scrb, scrl, scrr);
    }
}

void Combat::write(SaveLoadStream *buff)
{
    buff->writeBoolean(mIsEnable);
    if (mIsEnable)
    {
        int size = (int)mMonsterType.size();
        buff->writeInt(size);
        for (int i = 0; i < size; i++)
        {
            buff->writeInt(mMonsterType.at(i));
        }

        buff->writeInt(mScrb);
        buff->writeInt(mScrl);
        buff->writeInt(mScrR);
    }
}
