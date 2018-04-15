#pragma once

#include <iostream>
#include "Bitmap.h"
#include "BaseScreen.h"
#include "ScreenMagic.h"
#include "ScreenGoodsList.h"

using namespace std;

class  ScreenMenuGoods : public BaseScreen
{

private:
    Bitmap *mFrameBmp;
    string strs[2];
    int mSelId;

public:
    ScreenMenuGoods();
    virtual ~ScreenMenuGoods();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual bool isPopup(){return true;}
};
