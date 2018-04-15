#pragma once
#include <iostream>
#include "BaseScreen.h"
#include "BaseGoods.h"
#include "GoodsMedicine.h"

using namespace std;

class  ScreenTakeMedicine : public BaseScreen
{
private:
    static GoodsMedicine *sMedicine;
    int mStatePageIndex; // 人物属性页，共两页
    int mActorIndex;

public:
    ScreenTakeMedicine(BaseGoods *goods);
    ~ScreenTakeMedicine();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};
