#ifndef _Combat_h_
#define _Combat_h_

#include "BaseScreen.h"
#include "Random.h"
#include "Action.h"
#include "Monster.h"
#include "Player.h"
#include "CombatUI.h"
#include <iostream>
#include "HMQueue.h"

using namespace std;

class ActionExecutor;

class CombatSuccess;

class SaveLoadStream;

class  Combat : public BaseScreen, public CombatUI::CallBack
{
private:
    enum CombatState
    {
        SelectAction, // 玩家操作阶段，制定攻击策略
        PerformAction, // 执行动作队列，播放攻击动画
        Win, // 赢得战斗
        Loss, // 战斗失败
        Exit
    };
private:
    bool mIsEnable;
    bool mIsFighting;
    bool mIsRandomFight;
    static const int COMBAT_PROBABILITY = 20;;
    Random *mRandom;
    /** 玩家角色中心坐标*/
    Point mPlayerPos[3];

    int mScrb, mScrl, mScrR;
    CombatState mCombatState;
    /** 是否自动攻击，围攻状态*/
    bool mIsAutoAttack;
    /** 动作队列，一个回合中，双方的决策*/
    HMQueue<Action*> *mActionQueue;
    /** 动作队列的执行者*/
    ActionExecutor *mActionExecutor;
    /** 战斗的UI*/
    CombatUI *mCombatUI;
    /** 随机战斗中，可能出现的敌人类型*/
    vector<int> mMonsterType;
    /** 参加战斗的怪物队列*/
    vector<Monster*> mMonsterList;
    /** 参加战斗的玩家角色队列*/
    vector<Player*> mPlayerList;
    /** 当前选择动作的角色在
    {@link #mPlayerList}中的序号*/
    int mCurSelActionPlayerIndex;
    /** 当前回合*/
    int mRoundCnt;
    bool mHasEventExed;
    /** 最多回合数，0为无限*/
    int mMaxRound;
    /** 触发事件的回合，以及对应的事件号*/
    vector<int> mEventRound, mEventNum;
    /** 战斗失败跳转地址，战斗成功跳转地址*/
    int mLossAddr, mWinAddr;
    ResSrs *mFlyPeach;
    bool mIsWin;
    /**战斗背景图*/
    Bitmap *mBackground;
    /** 战斗胜利能获得的金钱和经验*/
    int mWinMoney, mWinExp;
    CombatSuccess *mCombatSuccess;
    long mTimeCnt;

private:
    Combat();
    ~Combat();
public:
    static Combat *getInstance();
    bool isActive();
     //开启随即战斗
    void fightEnable();
    //关闭随即战斗
    void fightDisable();

    /**
     * 初始化并开启随即战斗
     * @param monstersType 0-7 可能出现的敌人种类
     * @param scrb 战斗背景
     * @param scrl 左下角图
     * @param scrr 右上角图
     */
    void initFight(vector<int> monstersType, int scrb, int scrl, int scrr);

    /**
     * 剧情战斗
     * @param roundMax 最多回合数，0为无限
     * @param monstersType 0-3 敌人
     * @param scr 0战斗背景，1左下角图，2右上角图
     * @param evtRnds 0-3 战斗中，触发事件的回合
     * @param evts 0-3 对应的事件号
     * @param lossto 战斗失败跳转的地址
     * @param winto 战斗成功跳转的地址
     */
    void enterFight(int roundMax, int monstersType[3], int scr[3], int evtRnds[3], int evts[3], int lossto, int winto);

    /**
     * 进入一个随机战斗
     * @return <code>true</code>新战斗 <code>false</code>不开始战斗
     */
    bool startNewRandomCombat();

private:
    void createBackgroundBitmap(int scrb, int scrl, int scrr);
    void prepareForNewCombat();
    void exitCurrentCombat();
    void setOriginalPlayerPos();
    void setOriginalMonsterPos();
public:
    virtual void update(long delta);
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);

    void generateAutoActionQueue();
    void generateMonstersActions();

private:
    /** 按敏捷从大到小排列 */
    void sortActionQueue();
    /** 是否有玩家角色存活 */
    bool isAnyPlayerAlive();
    /** 怪物是否都挂了 */
    bool isAllMonsterDead();
    /** index 之后的主角是否都挂 */
    bool isPlayerBehindDead(int index);

    /**
    * 获取下一个存活的主角序号
    * @return 没有就返回-1
    */
    int getNextAlivePlayerIndex();
    int getPreAlivePlayerIndex();
    int getFirstAlivePlayerIndex();

public:
    /**
    *
    * @return 第一个活着的怪物，<code>NULL</code>怪物都挂了
    */
    Monster *getFirstAliveMonster();

    /**
    * 随机获取一个或者的玩家角色
    * @return <code>NULL</code>全死了
    */
    Player *getRandomAlivePlayer();

private:
    /** 更新双方状态*/
    void updateFighterState();

public:
    virtual void onActionSelected(Action *action);
    virtual void onAutoAttack();
    virtual void onFlee();
    virtual void onCancel();

public:
    void read(SaveLoadStream *buff);
    void write(SaveLoadStream *buff);

};

#endif