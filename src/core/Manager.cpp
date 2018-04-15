#include "Manager.h"
#include "GameView.h"

Manager::Manager()
{
    mMathod = NULL;
}

Manager::~Manager()
{

}

Manager * Manager::getInstance()
{
    static Manager instance;
    return &instance;
}

HMMathod * Manager::getMethod() const
{
    return mMathod;
}

void Manager::setMethod(HMMathod *mathod)
{
    if (mathod != NULL)
    {
        mMathod = mathod;
    }
}

void Manager::onKeyEvent(int type, int key)
{
    GameView *view = GameView::getInstance();
    if (!view->bInitiated)
    {
        return;
    }

    if (1 == type)
    {
        view->keyDown(key);
    }
    else
    {
        view->keyUp(key);
    }
}

long Manager::sCOLOR_WHITE = 0xFFFFFFFF;
long Manager::sCOLOR_BLACK = 0xFF000000;
long Manager::sCOLOR_TRANSP = 0x00000000;

