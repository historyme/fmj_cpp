#pragma once

#include <iostream>
#include <fstream>

#include "BaseScreen.h"
#include "HMStack.h"

class BaseGoods;
class Canvas;

class  ScreenGoodsList : public BaseScreen
{
public:
    enum Mode
    {
        Sale,
        Buy,
        Use,
    };

    struct OnItemSelectedListener
    {
        virtual void onItemSelected(BaseGoods *goods) = 0;
    };

private:
    Mode mMode;
    OnItemSelectedListener *mOnItemSelectedListener;
    vector<BaseGoods*> *mGoodsList;
    static Bitmap *sbmpBg;
    static Rect *mRectGoodsDsp;
    std::string mDescription;
    int mToDraw; // 当前要画的描述中的字节
    int mNextToDraw; // 下一个要画的描述中的字节
    HMStack<int> mStackLastToDraw; // 保存上次描述所画位置
    static const int ITEM_NUM = 4; // 界面上显示的条目数
    int mFirstItemIndex; // 界面上显示的第一个物品的序号
    int mCurItemIndex; // 当前光标所在位置物品的序号
    static Canvas *sCanvas;
    int mLastDownKey;
public:
    ScreenGoodsList(vector<BaseGoods*> *list, OnItemSelectedListener *l, Mode m);
    ~ScreenGoodsList();
    virtual void update(long delta);
    virtual void draw(Canvas *canvas);

private:
    void showNextItem();
    void showPreItem();

public:
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};
