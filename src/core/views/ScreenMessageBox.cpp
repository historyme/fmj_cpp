#include "ScreenMessageBox.h"

#include "Bitmap.h"
#include "Canvas.h"
#include "TextRender.h"
#include "GameView.h"

ScreenMessageBox::ScreenMessageBox(std::string msg, OnOKClickListener *l)
{
    mIndex = 0;

    bmpBg = Bitmap::createBitmap(137 - 27 + 1, 81 - 15 + 1, ARGB_8888);
    Canvas c(bmpBg);
    c.drawColor(Manager::sCOLOR_WHITE);
    Paint p;
    p.setColor(Manager::sCOLOR_BLACK);
    p.setStyle(Paint::STROKE);
    c.drawRect(1, 1, bmpBg->getWidth() - 5, bmpBg->getHeight() - 5, &p);
    c.drawRect(43 - 27, 51 - 15, 70 - 27, 70 - 15, &p);
    c.drawRect(91 - 27, 51 - 15, 118 - 27, 70 - 15, &p);
    p.setStyle(Paint::FILL_AND_STROKE);
    c.drawRect(32 - 27, 77 - 15, 137 - 27, 81 - 15, &p);
    c.drawRect(133 - 27, 20 - 15, bmpBg->getWidth() - 1, bmpBg->getHeight() - 1, &p);

    mOnOkClickListener = l;
    mMsg = msg;
}

ScreenMessageBox::~ScreenMessageBox()
{
    delete bmpBg;
    bmpBg = NULL;
}

void ScreenMessageBox::draw(Canvas *canvas)
{
    canvas->drawBitmap(bmpBg, 27, 15);
    TextRender::drawText(canvas, mMsg, 33, 23);
    if (mIndex == 0)
    {
        TextRender::drawSelText(canvas, "ÊÇ ", 45, 53);
        TextRender::drawText(canvas, "·ñ ", 93, 53);
    }
    else if (mIndex == 1)
    {
        TextRender::drawText(canvas, "ÊÇ ", 45, 53);
        TextRender::drawSelText(canvas, "·ñ ", 93, 53);
    }
}

void ScreenMessageBox::onKeyDown(int key)
{
    if (key == KEY_LEFT || key == KEY_RIGHT)
    {
        mIndex = 1 - mIndex;
    }
}

void ScreenMessageBox::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        if (mIndex == 0 && mOnOkClickListener != NULL)
        {
            mOnOkClickListener->onOKClick();
        }
        _exit();
    }
    else if (key == KEY_CANCEL)
    {
        _exit();
    }
}

bool ScreenMessageBox::isPopup()
{
    return true;
}

void ScreenMessageBox::_exit()
{
    GameView::getInstance()->popScreen();
}

