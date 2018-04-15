#pragma once
#include <iostream>
#include "Bitmap.h"
#include "BaseScreen.h"
#include "HMStack.h"
#include "Point.h"

class Rect;
class GoodsEquipment;

using namespace std;

class  ScreenActorWearing : public BaseScreen
{
private:
    Point mPos[8];
    GoodsEquipment *mEquipments[8];
    string mItemName[8];
    int mCurItem;
    int mActorIndex;
    bool showingDesc;
    Bitmap *bmpName;
    Bitmap *bmpDesc;
    string mTextName;
    string mTextDesc;
    int mToDraw; // 当前要画的描述中的字节
    int mNextToDraw; // 下一个要画的描述中的字节
    HMStack<int> mStackLastToDraw; // 保存上次描述所画位置
    static Rect *sRectDesc;

public:
    ScreenActorWearing();
    ~ScreenActorWearing();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);

private:
    vector<BaseGoods*> *getTheEquipList(int type);
    void resetDesc();
};
