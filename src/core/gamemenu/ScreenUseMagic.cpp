#include "ScreenUseMagic.h"
#include "ScreenMainGame.h"
#include "MagicRestore.h"

ScreenUseMagic::ScreenUseMagic(MagicRestore *magic, Player *scr)
    :mNameRect(new Rect)
{
    mNameRect->set(4, 4, 37, 96);

    mMagic = magic;
    mScr = scr;

    mCurPage = 0;
    mCurActor = 0;
}

ScreenUseMagic::~ScreenUseMagic()
{
    delete mNameRect;
}

void ScreenUseMagic::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    TextRender::drawText(canvas, mMagic->getMagicName(), 0, mNameRect);
    Player *actor = ScreenMainGame::sPlayerList[mCurActor];
    actor->drawState(canvas, mCurPage);
    actor->drawHead(canvas, 5, 60);
}

void ScreenUseMagic::onKeyDown(int key)
{
    if (key == KEY_RIGHT && mCurActor < ScreenMainGame::sPlayerList.size() - 1)
    {
        ++mCurActor;
    }
    else if (key == KEY_LEFT && mCurActor > 0)
    {
        --mCurActor;
    }
    else if (key == KEY_PAGEDOWN || key == KEY_PAGEUP)
    {
        mCurPage = 1 - mCurPage;
    }
}

void ScreenUseMagic::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        mMagic->use(mScr, ScreenMainGame::sPlayerList[mCurActor]);
        GameView::getInstance()->popScreen();
    }
}
