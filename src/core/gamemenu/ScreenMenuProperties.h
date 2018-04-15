#pragma once

#include <iostream>
#include "Bitmap.h"
#include "BaseScreen.h"

using namespace std;

class  ScreenMenuProperties : public BaseScreen
{
private:
    Bitmap *mFrameBmp;
    string strs[2];
    int mSelId;

public:
    ScreenMenuProperties();
    virtual ~ScreenMenuProperties();
    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual bool isPopup(){return true;}
};
