#include "ScreenGoodsList.h"
#include "BaseGoods.h"
#include "Canvas.h"
#include "Util.h"
#include "GameView.h"
#include "common.h"
#include "Player.h"

using namespace std;

Bitmap * ScreenGoodsList::sbmpBg = NULL;
Rect * ScreenGoodsList::mRectGoodsDsp = NULL;
Canvas * ScreenGoodsList::sCanvas = NULL;

ScreenGoodsList::ScreenGoodsList(vector<BaseGoods*> *list, OnItemSelectedListener *l, Mode m)
{
    if (l == NULL)
    {
        cout << "ScreenGoodsList construtor params can't be NULL." << endl;
        return;
    }

    mToDraw = 0; // 当前要画的描述中的字节
    mNextToDraw = 0; // 下一个要画的描述中的字节
    mFirstItemIndex = 0; // 界面上显示的第一个物品的序号
    mCurItemIndex = 0; // 当前光标所在位置物品的序号

    mGoodsList = list;
    mOnItemSelectedListener = l;
    mMode = m;

    if (mGoodsList->size() > 0)
    {
        mDescription = mGoodsList->at(0)->getDescription();
    }

    if (sbmpBg == NULL)
    {
        sbmpBg = Bitmap::createBitmap(160, 96, ARGB_8888);
        int pts[] = 
        {
            40, 21, 40, 95, 
            40, 95, 0, 95, 
            0, 95, 0, 5, 
            0, 5, 5, 0,
            5, 0, 39, 0,
            39, 0, 58, 19, 
            38, 0, 57, 19,
            57, 19, 140, 19,
            41, 20, 140, 20, 
            41, 21, 159, 21,
            54, 0, 140, 0,
            40, 95, 159, 95, 
            40, 57, 160, 57, 
            40, 58, 140, 58, 
            40, 59, 159, 59,
            41, 20, 41, 95,
            42, 20, 42, 95,
            159, 21, 159, 57,
            159, 59, 159, 96
        };

        if (sCanvas == NULL)
        {
            sCanvas = new Canvas(sbmpBg);
        }
        sCanvas->drawColor(Manager::sCOLOR_WHITE);
        sCanvas->drawLines(pts, 19, &Util::sBlackPaint);
        TextRender::drawText(sCanvas, "名:", 45, 23);
        TextRender::drawText(sCanvas, "价:", 45, 40);
    }
    mRectGoodsDsp = new Rect(44, 61, 156, 94);
    mLastDownKey = -1;
}

ScreenGoodsList::~ScreenGoodsList()
{
    delete mRectGoodsDsp;
}

void ScreenGoodsList::update(long delta)
{
    if (mGoodsList->size() <= 0)
    {
        GameView::getInstance()->popScreen();
    }
}

void ScreenGoodsList::draw(Canvas *canvas)
{
    canvas->drawBitmap(sbmpBg, 0, 0);
    if (mGoodsList->size() <= 0) return;

    while (mCurItemIndex >= mGoodsList->size())
        showPreItem();

    BaseGoods *g = mGoodsList->at(mCurItemIndex);
    TextRender::drawText(canvas, mMode == Buy ? string("金钱:") + int_to_string(Player::sMoney) : string("数量:") + int_to_string(g->getGoodsNum()), 60, 2);
    TextRender::drawText(canvas, g->getName(), 69, 23);
    TextRender::drawText(canvas, string("") + (mMode == Buy ? int_to_string(g->getBuyPrice()) : int_to_string(g->getSellPrice())), 69, 40);
    Util::drawTriangleCursor(canvas, 4, 8 + 23 * (mCurItemIndex - mFirstItemIndex));

    for (int i = mFirstItemIndex; i < mFirstItemIndex + ITEM_NUM && i < mGoodsList->size(); i++)
    {
        mGoodsList->at(i)->draw(canvas, 14, 2 + 23 * (i - mFirstItemIndex));
    }

    mNextToDraw = TextRender::drawText(canvas, mDescription, mToDraw, mRectGoodsDsp);
}

void ScreenGoodsList::showNextItem()
{
    ++mCurItemIndex;
    mDescription = mGoodsList->at(mCurItemIndex)->getDescription();
    if (mCurItemIndex >= mFirstItemIndex + ITEM_NUM)
    {
        ++mFirstItemIndex;
    }
    mToDraw = mNextToDraw = 0;
    mStackLastToDraw.clear();
}

void ScreenGoodsList::showPreItem()
{
    --mCurItemIndex;
    mDescription = mGoodsList->at(mCurItemIndex)->getDescription();
    if (mCurItemIndex < mFirstItemIndex)
    {
        --mFirstItemIndex;
    }
    mToDraw = mNextToDraw = 0;
    mStackLastToDraw.clear();
}

void ScreenGoodsList::onKeyDown(int key)
{
    if (key == KEY_UP && mCurItemIndex > 0)
    {
        showPreItem();
    }
    else if (key == KEY_DOWN && mCurItemIndex + 1 < mGoodsList->size())
    {
        showNextItem();
    }
    else if (key == KEY_PAGEDOWN)
    {
        int len = mDescription.size();
        if (mNextToDraw < len)
        {
            mStackLastToDraw.push(mToDraw);
            mToDraw = mNextToDraw;
        }
    }
    else if (key == KEY_PAGEUP && mToDraw != 0)
    {
        if (!mStackLastToDraw.empty())
        {
            mToDraw = mStackLastToDraw.pop();
        }
    }
    mLastDownKey = key;
}

void ScreenGoodsList::onKeyUp(int key)
{
    if (key == KEY_ENTER && mLastDownKey == KEY_ENTER)
    {
        mOnItemSelectedListener->onItemSelected(mGoodsList->at(mCurItemIndex));
    }
    else if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
}
