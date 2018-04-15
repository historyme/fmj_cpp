#include "ScreenMenuProperties.h"
#include "TextRender.h"
#include "GameView.h"
#include "ScreenActorState.h"
#include "ScreenActorWearing.h"

ScreenMenuProperties::ScreenMenuProperties()
{
    mFrameBmp = Util::getFrameBitmap(77 - 39 + 1, 54 - 16 + 1);
    strs[0] = "×´Ì¬";
    strs[1] = "´©´÷";
    mSelId = 0;
}

ScreenMenuProperties::~ScreenMenuProperties()
{
    delete mFrameBmp;
}

void ScreenMenuProperties::draw(Canvas *canvas)
{
    canvas->drawBitmap(mFrameBmp, 39, 16);
    if (mSelId == 0)
    {
        TextRender::drawSelText(canvas, strs[0], 39 + 3, 16 + 3);
        TextRender::drawText(canvas, strs[1], 39 + 3, 16 + 3 + 16);
    }
    else if (mSelId == 1)
    {
        TextRender::drawText(canvas, strs[0], 39 + 3, 16 + 3);
        TextRender::drawSelText(canvas, strs[1], 39 + 3, 16 + 3 + 16);
    }
}

void ScreenMenuProperties::onKeyDown(int key)
{
    if (key == KEY_UP || key == KEY_DOWN)
    {
        mSelId = 1 - mSelId;
    }
}

void ScreenMenuProperties::onKeyUp(int key)
{
    int selId = mSelId;

    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        GameView::getInstance()->popScreen();
        if (selId == 0)
        {
            GameView::getInstance()->pushScreen(new ScreenActorState());
        }
        else
        {
            GameView::getInstance()->pushScreen(new ScreenActorWearing());
        }
    }
}
