#pragma once

#include "Player.h"
#include "BaseGoods.h"
#include "BaseScreen.h"

using namespace std;

class  CombatSuccess
{
private:
    vector<BaseGoods*> mGoodsList;
    vector<BaseScreen*> mMsgList;
    vector<BaseScreen*> mLvupList;
    long mCnt;
    bool mIsAnyKeyPressed;

public:
    CombatSuccess(int exp, int money, vector<BaseGoods*> goodslist, vector<Player*> lvuplist);
    ~CombatSuccess();
    /**
     *
     * @param delta
     * @return <code>true</code> 所以内容显示完毕
     */
    bool update(long delta);
    void draw(Canvas *canvas);
    void onKeyDown(int key){}
    void onKeyUp(int key){mIsAnyKeyPressed = true;}
};

/**
    * 显示战斗胜利后玩家获得的东西
    * @author Chen
    *
    */
class MsgScreen : public BaseScreen
{
private:
    ResImage *mSide;
    Bitmap *mMsg;
    int mX, mY;

public:
    MsgScreen(std::string _msg, int y = (96 - 24) / 2);
    ~MsgScreen();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key){}
    virtual void onKeyUp(int key){}
};


class LevelupScreen : public BaseScreen
{
private:
    ResImage *mSide;
    Bitmap *mInfo;

public:
    LevelupScreen(Player *p);
    ~LevelupScreen();
    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key){}
    virtual void onKeyUp(int key){}
};


class LearnMagicScreen : public BaseScreen
{
private:
    ResImage *mSide;
    Bitmap *mInfo;
public:
    LearnMagicScreen(std::string playerName, string magicName);
    ~LearnMagicScreen();
    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key){}
    virtual void onKeyUp(int key){}
};



