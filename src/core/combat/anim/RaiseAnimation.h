#ifndef _RaiseAnimation_h_
#define _RaiseAnimation_h_

#include <iostream>
#include <list>

#include "Canvas.h"
#include "FightingCharacter.h"
#include "Canvas.h"
#include "Util.h"
#include "DatLib.h"
#include "ResSrs.h"
#include "Bitmap.h"


class RaiseAnimation
{
private:
    int x;
    int y;

    int dy;
    int dt;

    Bitmap *raiseNum;
    std::list<ResSrs *> srsList;
    bool bShowNum;
    long cnt;
public:
    RaiseAnimation(int x, int y, int hitpoint, int buff);

    ~RaiseAnimation();

    bool update(long delta);

    void draw(Canvas *canvas);

};

#endif
