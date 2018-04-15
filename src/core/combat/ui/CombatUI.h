#ifndef _CombatUI_h_
#define _CombatUI_h_

#include <iostream>
#include "Global.h"
#include "BaseScreen.h"
#include "Point.h"
#include "Player.h"
#include "Monster.h"
#include "Action.h"
#include "Paint.h"
#include "Rect.h"
#include "Canvas.h"
#include "HMStack.h"

using namespace std;

class FrameAnimation;
class CombatUI;

/**
* @see MenuCharacterSelect
*/
struct OnCharacterSelectedListener
{
    /**
    *
    * @param fc 被选择的角色
    */
    virtual void onCharacterSelected(FightingCharacter *fc) = 0;
};

/** 角色标识，用于标记当前选择的角色 */
class MenuCharacterSelect : public BaseScreen
{
private:
    OnCharacterSelectedListener *mOnCharacterSelectedListener;
    FrameAnimation *mIndicator;
    vector<Point> mIndicatorPos;
    vector<FightingCharacter*> mList;
    int mCurSel;
    bool mIgnoreDead;

public:
    /**
    *
    * @param indicator 标记符的帧动画
    * @param pos 标记符的位置
    * @param list 角色链表
    * @param ignoreDead 跳过死亡角色
    */
    MenuCharacterSelect(FrameAnimation *indicator,
        Point pos[3],
        vector<FightingCharacter*> list,
        OnCharacterSelectedListener *l,
        bool ignoreDead);

    virtual ~MenuCharacterSelect();

    virtual void update(long delta);
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);

private:
    void selectNextTarget();
    void selectPreTarget();
};

 /** 显示主菜单、角色信息 */
 class MainMenu : public BaseScreen
 {
 private:
     /* 1↑、2←、3↓、4→ */
     ResImage *mMenuIcon;
     /* 显示角色HP MP的背景图 */
     ResImage *mPlayerInfoBg;
     int mCurIconIndex;
 
 public:
     MainMenu();
     virtual ~MainMenu();
     virtual void update(long delta);
     virtual void draw(Canvas *canvas);
     virtual void onKeyDown(int key);
     virtual void onKeyUp(int key);
 };
 
/** 围攻、道具、防御、逃跑、状态 */
class MenuMisc : public BaseScreen
{
private:
    Bitmap *mBg;
    string mText;
    vector<string> mItemText;
    Rect *mTextRect;
    int mCurSelIndex;
public:
    MenuMisc();
    virtual ~MenuMisc();
    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};

/** 战斗中，显示玩家异常状态 */
class MenuState : public BaseScreen
{
private:
    ResImage *mBg;
    /** 1↑2↓3×4√5回 */
    ResImage *mMarker;
    int mCurPlayer;

public:
    MenuState();
    virtual ~MenuState();
    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};

/** 道具子菜单，装备、投掷、使用 */
class MenuGoods : public BaseScreen
{
private:
    Bitmap *mBg;
    string mText;
    vector<string> mItemText;
    Rect *mTextRect;
    int mSelIndex;
public:
    MenuGoods();
    virtual ~MenuGoods();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);

private:
    /** 当前物品链表中，可用物品 */
    vector<BaseGoods *> *getUseableGoodsList();
    /** 当前物品链表中，可用于投掷敌人的物品 */
    vector<BaseGoods *> *getThrowableGoodsList();
    static void equipSelected(BaseGoods *goods);
};

class CombatUI : public BaseScreen
{
public:
    struct CallBack
    {
        /**
        * 当一个Action被选择后，会调用此方法
        * @param action 选择的Action
        */
        virtual void onActionSelected(Action *action) = 0;

        /**
        * 选择围攻时，调用改方法
        */
        virtual void onAutoAttack() = 0;

        /**
        * 选择逃跑时，调用该方法。对于已经做出决策的角色，其决策不变；之后的角色动作皆为逃跑
        */
        virtual void onFlee() = 0;

        /**
        * 取消选择当前角色的Action，应该返回选择上一个角色的Action
        */
        virtual void onCancel() = 0;
    };

public:
    CallBack *mCallBack;

    Point mPlayerIndicatorPos[3];
    Point mMonsterIndicatorPos[3];

    HMStack<BaseScreen *> mScreenStack;
    vector<FightingCharacter *> mPlayerList;
    vector<FightingCharacter *> mMonsterList;
    int mCurPlayerIndex;
    /** 标记发出的action的玩家角色 */
    FrameAnimation *mPlayerIndicator;
    /** 标记action作用的玩家角色 */
    FrameAnimation *mTargetIndicator;
    /** 标记action作用的敌人角色 */
    FrameAnimation *mMonsterIndicator;
    ResImage *mHeadsImg[3];

public:
    static CombatUI *getInstance();

    CombatUI();
    virtual ~CombatUI() {}
    virtual void update(long delta);
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    void reset();
    void setPlayerList(std::vector<Player *> list);
    void setMonsterList(std::vector<Monster *> list);
    void setCurrentPlayerIndex(int i);
    void setCallBack(CallBack *callBack);
    Player *getCurPlayer() const;

public:
    /** helper for the callback interface */
    void onActionSelected(Action *action);
    /** helper for the callback interface */
    void onCancel();
};


#endif