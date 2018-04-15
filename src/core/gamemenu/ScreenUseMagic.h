#ifndef _ScreenUseMagic_H_
#define _ScreenUseMagic_H_

#include <iostream>
#include "BaseGoods.h"
#include "GameView.h"
#include "TextRender.h"
#include "BaseScreen.h"
#include "Bitmap.h"
#include "canvas.h"
#include "Point.h"
#include "Rect.h"
#include "MagicRestore.h"
#include "Player.h"

class ScreenUseMagic: public BaseScreen
{
public:
    ScreenUseMagic(MagicRestore *magic, Player *scr);
    virtual ~ScreenUseMagic();

    virtual void update(long delta){}

    virtual  void draw(Canvas *canvas);

    virtual  void onKeyDown(int key);

    virtual  void onKeyUp(int key);

private:
    Rect *mNameRect;

    MagicRestore *mMagic;
    Player *mScr;

    int mCurPage;
    int mCurActor;

};
#endif