#include "ScreenAnimation.h"
#include "GameView.h"

ScreenAnimation::ScreenAnimation(int index)
{
    if (index != 247 && index != 248 && index != 249)
    {
        std::cout << "只能是247,248,249" << std::endl;
        exit(0);
    }
    mIndex = index;
    mResSrs = (ResSrs *) DatLib::getInstance()->getRes(DatLib::RES_SRS, 1, index);
    mResSrs->setIteratorNum(4);
    mResSrs->startAni();
}

ScreenAnimation::~ScreenAnimation()
{
    delete mResSrs;
}

void ScreenAnimation::update(long delta)
{
    if (!mResSrs->update(delta))
    {
        if (mIndex == 247)   // 转到游戏动画
        {
            GameView::getInstance()->changeScreen(SCREEN_GAME_LOGO);
        }
        else if (mIndex == 248)     // 转到游戏菜单
        {
            GameView::getInstance()->changeScreen(SCREEN_MENU);
        }
        else if (mIndex == 249)     //
        {
            GameView::getInstance()->changeScreen(SCREEN_MENU);
        }
    }
}

void ScreenAnimation::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    mResSrs->draw(canvas, 0, 0);
}

void ScreenAnimation::onKeyDown(int key)
{
    if (key == KEY_CANCEL && (mIndex == 247 || mIndex == 248))
    {
        GameView::getInstance()->changeScreen(SCREEN_MENU);
    }
}

void ScreenAnimation::onKeyUp(int key)
{
}