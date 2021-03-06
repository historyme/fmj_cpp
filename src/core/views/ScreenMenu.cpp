#include "ScreenMenu.h"
#include "GameView.h"


ScreenMenu::ScreenMenu() :
    isCancelKeyDown(false),
    mCurSelect(1)
{
    mImgMenu = (ResImage *)DatLib::getInstance()->getRes(DatLib::RES_PIC, 2, 14);

    mSrsSelector[0] = (ResSrs *)DatLib::getInstance()->getRes(DatLib::RES_SRS, 1, 250);
    mSrsSelector[1] = (ResSrs *)DatLib::getInstance()->getRes(DatLib::RES_SRS, 1, 251);
    mSrsSelector[0]->startAni();
    mSrsSelector[1]->startAni();
    mLeft = (160 - mImgMenu->getWidth()) / 2;
    mTop = (96 - mImgMenu->getHeight()) / 2;
}

ScreenMenu::~ScreenMenu()
{
    delete mImgMenu;
    delete mSrsSelector[0];
    delete mSrsSelector[1];
}

void ScreenMenu::update(long delta)
{
    if (!mSrsSelector[mCurSelect]->update(delta))
    {
        mSrsSelector[mCurSelect]->startAni();
    }
}

void ScreenMenu::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    mImgMenu->draw(canvas, 1, mLeft, mTop);
    mSrsSelector[mCurSelect]->draw(canvas, 0, 0);
}

void ScreenMenu::onKeyDown(int key)
{
    switch (key)
    {
    case KEY_UP:
    case KEY_DOWN:
        mCurSelect = 1 - mCurSelect;
        break;
    case KEY_CANCEL:
        isCancelKeyDown = true;
        break;
    }
}

void ScreenMenu::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        if (mCurSelect == 0)   // 新游戏
        {
            SaveLoadGame::startNewGame = true;
            GameView::getInstance()->changeScreen(SCREEN_MAIN_GAME);
        }
        else if (mCurSelect == 1)     // 读取进度
        {
            GameView::getInstance()->pushScreen(new ScreenSaveLoadGame(ScreenSaveLoadGame::LOAD));
        }
    }
    else if (key == KEY_CANCEL && isCancelKeyDown)
    {
        std::cout << "退出游戏";
        exit(0);
    }
}
