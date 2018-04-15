#include "ScreenMenuSystem.h"


ScreenMenuSystem::ScreenMenuSystem()
{
    first = 0;
    index = 0;
    str[0] = "读入进度";
    str[1] = "存储进度";
    str[2] = "游戏设置";
    str[3] = "结束游戏";

    strX = 42;
    strY = 32;
    selY = strY;

    bmpFrame = Util::getFrameBitmap(109 - 39 + 1, 91 - 29 + 1);
    bmpArrowUp = Bitmap::createBitmap(8, 4, ARGB_8888);
    bmpArrowDown = Bitmap::createBitmap(8, 4, ARGB_8888);
    bmpArr[0] = bmpArrowDown;
    bmpArr[1] = bmpArrowUp;

    arrowX = 70;
    arrowY = 82;
    bmpi = 0;


    Paint p;
    p.setColor(Manager::sCOLOR_BLACK);

    Canvas c(bmpArrowUp);

    c.drawColor(Manager::sCOLOR_WHITE);
    c.drawLine(3, 0, 4, 0, &p);
    c.drawLine(2, 1, 5, 1, &p);
    c.drawLine(1, 2, 6, 2, &p);
    c.drawLine(0, 3, 7, 3, &p);

    c.setBitmap(bmpArrowDown);
    c.drawColor(Manager::sCOLOR_WHITE);
    c.drawLine(0, 0, 7, 0, &p);
    c.drawLine(1, 1, 6, 1, &p);
    c.drawLine(2, 2, 5, 2, &p);
    c.drawLine(3, 3, 4, 3, &p);
}

ScreenMenuSystem::~ScreenMenuSystem()
{
    delete bmpFrame;
    delete bmpArrowUp;
    delete bmpArrowDown;
}

void ScreenMenuSystem::draw(Canvas *canvas)
{
    canvas->drawBitmap(bmpFrame, 39, 29);
    TextRender::drawText(canvas, str[first], strX, strY);
    TextRender::drawText(canvas, str[first + 1], strX, strY + 16);
    TextRender::drawText(canvas, str[first + 2], strX, strY + 32);
    TextRender::drawSelText(canvas, str[index], strX, selY);
    canvas->drawBitmap(bmpArr[bmpi], arrowX, arrowY);
}

void ScreenMenuSystem::onKeyDown(int key)
{
    if (key == KEY_UP)
    {
        --index;
        selY -= 16;
    }
    else if (key == KEY_DOWN)
    {
        ++index;
        selY += 16;
    }

    if (index == 0 || index == 4)
    {
        index = 0;
        selY = 32;
        arrowY = 82;
        bmpi = 0;
        first = 0;
        strY = 32;
    }
    else if (index == 3 || index == -1)
    {
        index = 3;
        selY = 72;
        arrowY = 34;
        bmpi = 1;
        first = 1;
        strY = 40;
    }
}

void ScreenMenuSystem::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        switch (index)
        {
        case 0:
            GameView::getInstance()->pushScreen(new ScreenSaveLoadGame(ScreenSaveLoadGame::LOAD));
            break;

        case 1:
            GameView::getInstance()->pushScreen(new ScreenSaveLoadGame(ScreenSaveLoadGame::SAVE));
            break;

        case 2:
            break;

        case 3:
            GameView::getInstance()->changeScreen(SCREEN_MENU);
            break;
        }
    }
}
