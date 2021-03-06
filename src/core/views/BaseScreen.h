#ifndef _BaseScreen_h_
#define _BaseScreen_h_

#include "Canvas.h"
#include "ResSrs.h"
#include "DatLib.h"

class BaseScreen
{

public:
    BaseScreen()
    {
        bInstanceof_ScreenMainGame = false;
    }
    virtual ~BaseScreen(){};
    virtual void update(long delta) = 0;
    virtual void draw(Canvas *canvas) = 0;
    virtual void onKeyDown(int key) = 0;
    virtual void onKeyUp(int key) = 0;
    virtual bool isPopup(){return false;}
public:
    bool bInstanceof_ScreenMainGame;
};

#endif