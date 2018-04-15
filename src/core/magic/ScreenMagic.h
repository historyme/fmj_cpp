#ifndef _ScreenMagic_H_
#define _ScreenMagic_H_

#include <iostream>
#include "BaseGoods.h"

#include "GameView.h"
#include "TextRender.h"
#include "BaseScreen.h"

#include "Bitmap.h"
#include "canvas.h"
#include "Point.h"
#include "Rect.h"
#include "Paint.h"
#include "BaseMagic.h"

class ResMagicChain;

struct OnItemSelectedListener
{
    virtual ~OnItemSelectedListener(){}
    virtual void onItemSelected(BaseMagic *magic) = 0;
};

class ScreenMagic: public BaseScreen
{
public :
    ScreenMagic(ResMagicChain *magicChain, OnItemSelectedListener *l);
    virtual ~ScreenMagic();
private:
    void createBmp();

public:
    virtual void update(long delta){}

    virtual void draw(Canvas *canvas);

    virtual void onKeyDown(int key);

    virtual void onKeyUp(int key);

private:
    OnItemSelectedListener *mOnItemSelectedListener;

    ResMagicChain *mMagicChain;

    static const int ITEM_NUM = 2; // 界面上显示的条目数

    int mFirstItemIndex; // 界面上显示的第一个魔法的序号
    int mCurItemIndex; // 当前光标所在位置魔法的序号

    Bitmap *mBmpCursor;
    Bitmap *mBmpMarker;
    Bitmap *mBmpMarker2;

    Rect mRectTop;
    Rect mRectBtm;
    Rect mRectDsp;

    int mToDraw; // 当前要画的魔法描述中的字节
    int mNextToDraw; // 下一个要画的魔法描述中的字节

    std::stack<int> mStackLastToDraw; // 保存上次魔法描述所画位置

    Point mTextPos;
    Paint mFramePaint;

};

#endif
