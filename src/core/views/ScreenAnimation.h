#ifndef _ScreenAnimation_h_
#define _ScreenAnimation_h_

#include "BaseScreen.h"
#include <iostream>

class ScreenAnimation: public BaseScreen
{
public :

    /**
     *
     * @param index
     *            247、248、249分别代表游戏开发组的商标、游戏的名称 以及游戏战斗失败后的过场动画
     */
    ScreenAnimation(int index);
    virtual ~ScreenAnimation();

    virtual  void update(long delta);
    virtual  void draw(Canvas *canvas);
    virtual  void onKeyDown(int key);
    virtual  void onKeyUp(int key);


private:
    ResSrs *mResSrs;
    int mIndex;

};

#endif