#include "ScreenGameMainMenu.h"
#include "ScreenUseMagic.h"
#include "ScreenMenuSystem.h"
#include "ScreenMenuGoods.h"
#include "ScreenMenuProperties.h"

ScreenGameMainMenu::ScreenGameMainMenu()
{
    bmpFrame1 = Util::getFrameBitmap(93, 16 + 6);
    bmpFrame2 = Util::getFrameBitmap(32 + 6, 64 + 6);
    menuItemsS[0] = "属性";
    menuItemsS[1] = "魔法";
    menuItemsS[2] = "物品";
    menuItemsS[3] = "系统";
    mSelIndex = 0;

    screenSelectActor = new _screenSelectActor();

    menuItems = "属性魔法物品系统";

    menuItemsRect.left = 9 + 3;
    menuItemsRect.top = 3 + 16 + 6 - 1 + 3;
    menuItemsRect.right = menuItemsRect.left + 32;
    menuItemsRect.bottom = menuItemsRect.top + 64;
}

ScreenGameMainMenu::~ScreenGameMainMenu()
{
    delete screenSelectActor;
    delete bmpFrame1;
    delete bmpFrame2;
}

void ScreenGameMainMenu::draw(Canvas *canvas)
{
    canvas->drawBitmap(bmpFrame1, 9, 3);
    TextRender::drawText(canvas, std::string("金钱:") + int_to_string(Player::sMoney), 9 + 3, 3 + 3);
    canvas->drawBitmap(bmpFrame2, 9, 3 + 16 + 6 - 1);
    TextRender::drawText(canvas, menuItems, 0, &menuItemsRect);
    TextRender::drawSelText(canvas, menuItemsS[mSelIndex], menuItemsRect.left,
        menuItemsRect.top + mSelIndex * 16);
}

void ScreenGameMainMenu::onKeyDown(int key)
{
    if (key == KEY_UP)
    {
        if (--mSelIndex < 0)
        {
            mSelIndex = 3;
        }
    }
    else if (key == KEY_DOWN)
    {
        if (++mSelIndex > 3)
        {
            mSelIndex = 0;
        }
    }
}

void ScreenGameMainMenu::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        BaseScreen *screen = NULL;
        switch (mSelIndex)
        {
        case 0:
            screen = new ScreenMenuProperties();
            break;
        case 1:
            screen = ScreenMainGame::sPlayerList.size() > 1 ?
            screenSelectActor : getScreenMagic(0);
            break;
        case 2:
            screen = new ScreenMenuGoods();
            break;
        case 3:
            screen = new ScreenMenuSystem();
            break;
        }
        if (screen != NULL)
        {
            GameView::getInstance()->pushScreen(screen);
        }
    }
    else if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
}

ScreenMagic * ScreenGameMainMenu::getScreenMagic(const int id)
{
    OnMainMenuItemSelectedListener *Listener = new OnMainMenuItemSelectedListener(id);
    return new ScreenMagic((ScreenMainGame::sPlayerList)[id]->getMagicChain(), Listener);
}

void OnMainMenuItemSelectedListener::onItemSelected(BaseMagic *magic)
{
    if (magic->bMagicRestore)
    {
        GameView::getInstance()->pushScreen(
            new ScreenUseMagic((MagicRestore *)magic,
            (ScreenMainGame::sPlayerList)[id]));
    }
    else
    {
        Util::showMessage("此处无法使用!", 1000);
    }
}

OnMainMenuItemSelectedListener::OnMainMenuItemSelectedListener(int id)
{
    this->id = id;
}

OnMainMenuItemSelectedListener::~OnMainMenuItemSelectedListener()
{

}

_screenSelectActor::_screenSelectActor()
{
    mIndex = 0;

    mFrameRect.set(39, 29, 125, 67 - 32 + ScreenMainGame::sPlayerList.size() * 16);
    bmpFrame = Util::getFrameBitmap(mFrameRect.width(), mFrameRect.height());

    std::vector<Player *> list = ScreenMainGame::sPlayerList;
    mSum = list.size();
    mNames.resize(mSum);
    for (int i = 0; i < mSum; i++)
    {
        mNames[i] = list[i]->getName();
    }
}

void _screenSelectActor::draw(Canvas *canvas)
{
    canvas->drawBitmap(bmpFrame, mFrameRect.left, mFrameRect.top);
    for (int i = 0; i < mSum; i++)
    {
        if (i == mIndex)
        {
            TextRender::drawSelText(canvas, mNames[i], mFrameRect.left + 3,
                mFrameRect.top + 3 + 16 * i);
        }
        else
        {
            TextRender::drawText(canvas, mNames[i], mFrameRect.left + 3,
                mFrameRect.top + 3 + 16 * i);
        }
    }
}

void _screenSelectActor::onKeyDown(int key)
{
    if (key == KEY_DOWN)
    {
        ++mIndex;
        if (mIndex >= mSum)
        {
            mIndex = 0;
        }
    }
    else if (key == KEY_UP)
    {
        --mIndex;
        if (mIndex < 0)
        {
            mIndex = mSum - 1;
        }
    }
}

void _screenSelectActor::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        GameView::getInstance()->popScreen();
        GameView::getInstance()->pushScreen(ScreenGameMainMenu::getScreenMagic(mIndex));
    }
}

_screenSelectActor::~_screenSelectActor()
{
    delete bmpFrame;
}
