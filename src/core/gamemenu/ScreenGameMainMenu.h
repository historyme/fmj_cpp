#ifndef _ScreenGameMainMenu_H_
#define _ScreenGameMainMenu_H_

#include <iostream>
#include "BaseGoods.h"

#include "GameView.h"
#include "Global.h"
#include "Player.h"
#include "TextRender.h"
#include "Util.h"
#include "BaseMagic.h"
#include "MagicAttack.h"
#include "MagicRestore.h"
#include "ScreenMagic.h"
#include "ScreenMainGame.h"
#include "BaseScreen.h"

#include "Bitmap.h"
#include "Canvas.h"
#include "Rect.h"

struct OnMainMenuItemSelectedListener :public OnItemSelectedListener
{
    int id;
    OnMainMenuItemSelectedListener(int id);
    virtual ~OnMainMenuItemSelectedListener();
    virtual void onItemSelected(BaseMagic *magic);
};

class _screenSelectActor : public BaseScreen
{
private:
    int mIndex;

    Rect mFrameRect;

    Bitmap *bmpFrame;

    std::vector<std::string> mNames;

    int mSum;

public:
    _screenSelectActor();

    virtual ~_screenSelectActor();

    virtual void update(long delta){}

    virtual void draw(Canvas *canvas);

    virtual void onKeyDown(int key);

    virtual void onKeyUp(int key);

    virtual bool isPopup(){ return true; }
};

class ScreenGameMainMenu: public  BaseScreen
{
public :
    ScreenGameMainMenu();
    virtual ~ScreenGameMainMenu();
    virtual void update(long delta){}

    virtual void draw(Canvas *canvas);

    virtual void onKeyDown(int key);

    virtual void onKeyUp(int key);

    /**
    *
    * @param id 0 1 2
    * @return
    */
    static ScreenMagic *getScreenMagic(const int id);

private:
    virtual bool isPopup()
    {
        return true;
    }

private:
    Bitmap *bmpFrame1;
    Bitmap *bmpFrame2;
    Rect menuItemsRect;
    char *menuItems;
    std::string menuItemsS[4];

    int mSelIndex;

    BaseScreen *screenSelectActor;
};

#endif
