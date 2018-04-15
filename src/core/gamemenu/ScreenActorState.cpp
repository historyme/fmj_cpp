#include "ScreenActorState.h"
#include "ScreenMainGame.h"
#include "GameView.h"

ScreenActorState::ScreenActorState()
{
    mPage = 0;
    mPlayerList = ScreenMainGame::sPlayerList;
    mCurPlayer = 0;
}

ScreenActorState::~ScreenActorState()
{

}

void ScreenActorState::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    int i = 0;
    while (i < mPlayerList.size())
    {
        mPlayerList.at(i)->drawHead(canvas, 10, 2 + 32 * i);
        ++i;
    }
    if (mPlayerList.size() > 0)
    {
        mPlayerList.at(mCurPlayer)->drawState(canvas, mPage);
        Util::drawTriangleCursor(canvas, 3, 10 + 32 * mCurPlayer);
    }
}

void ScreenActorState::onKeyDown(int key)
{
    if (key == KEY_PAGEDOWN || key == KEY_PAGEUP)
    {
        mPage = 1 - mPage;
    }
    else if (key == KEY_DOWN)
    {
        ++mCurPlayer;
        if (mCurPlayer >= mPlayerList.size())
        {
            mCurPlayer = 0;
        }
    }
    else if (key == KEY_UP)
    {
        --mCurPlayer;
        if (mCurPlayer < 0)
        {
            mCurPlayer = mPlayerList.size() - 1;
        }
    }
}

void ScreenActorState::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
}