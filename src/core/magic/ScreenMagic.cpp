#include "ScreenMagic.h"
#include "ResMagicChain.h"

ScreenMagic::ScreenMagic(ResMagicChain *magicChain, OnItemSelectedListener *l)
{
    mFirstItemIndex = 0;
    mCurItemIndex = 0;
    mTextPos.set(10, 77);

    mBmpCursor = Bitmap::createBitmap(12, 12, ARGB_8888);
    mBmpMarker = Bitmap::createBitmap(5, 8, ARGB_8888);
    mBmpMarker2 = Bitmap::createBitmap(5, 8, ARGB_8888);

    mRectTop.set(10, 4, 147, 39);
    mRectBtm.set(10, 41, 147, 76);
    mRectDsp.set(11, 42, 146, 75);

    mToDraw = 0;
    mNextToDraw = 0;

    if (magicChain == NULL || l == NULL)
    {
        std::cout << "Invalid input" << std::endl;
        exit(0);
    }
    mMagicChain = magicChain;
    mOnItemSelectedListener = l;
    mFramePaint.setColor(Manager::sCOLOR_BLACK);
    mFramePaint.setStyle(Paint::STROKE);
    mFramePaint.setStrokeWidth(1);

    createBmp();
}

ScreenMagic::~ScreenMagic()
{
    delete mBmpCursor;
    delete mBmpMarker;
    delete mBmpMarker2;

    if (NULL != mOnItemSelectedListener)
    {
        delete mOnItemSelectedListener;
    }
}

void ScreenMagic::createBmp()
{
    Canvas canvas(NULL);
    Paint p;
    p.setColor(Manager::sCOLOR_BLACK);
    p.setStrokeWidth(1);
    p.setStyle(Paint::STROKE);

    canvas.setBitmap(mBmpCursor);
    canvas.drawColor(Manager::sCOLOR_WHITE);
    canvas.drawLine(8, 0, 11, 0, &p);
    canvas.drawLine(11, 1, 11, 4, &p);
    canvas.drawRect(6, 1, 7, 4, &p);
    canvas.drawRect(7, 4, 10, 5, &p);
    canvas.drawLine(7, 4, 0, 11, &p);
    canvas.drawLine(8, 5, 2, 11, &p);

    int pts[28] = {
        2, 0, 4, 2,
        4, 2, 4, 6,
        4, 6, 2, 7,
        2, 7, 0, 5,
        0, 5, 0, 2,
        0, 3, 3, 0,
        2, 3, 2, 5 };

    canvas.setBitmap(mBmpMarker);
    canvas.drawColor(Manager::sCOLOR_WHITE);
    canvas.drawLines(pts, 7, &p);

    canvas.setBitmap(mBmpMarker2);
    canvas.drawColor(Manager::sCOLOR_WHITE);
    canvas.drawLines(pts, 7, &p);

    int pts2[12] = {
        1, 1, 1, 6,
        2, 0, 2, 7,
        3, 2, 3, 6 };

    canvas.drawLines(pts2, 3, &p);
}

void ScreenMagic::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    canvas->drawRect(&mRectTop, &mFramePaint);
    canvas->drawRect(&mRectBtm, &mFramePaint);
    TextRender::drawText(canvas, mMagicChain->getMagic(mFirstItemIndex)->getMagicName(), mRectTop.left + 1, mRectTop.top + 1);
    if (mFirstItemIndex + 1 < mMagicChain->getLearnNum())
    {
        TextRender::drawText(canvas, mMagicChain->getMagic(mFirstItemIndex + 1)->getMagicName(), mRectTop.left + 1, mRectTop.top + 1 + 16);
    }
    mNextToDraw = TextRender::drawText(canvas, mMagicChain->getMagic(mCurItemIndex)->getMagicDescription(), mToDraw, &mRectDsp);
    TextRender::drawText(canvas, std::string("耗真气:") + int_to_string(mMagicChain->getMagic(mCurItemIndex)->getCostMp()), mTextPos.x, mTextPos.y);
    canvas->drawBitmap(mBmpCursor, 100, mFirstItemIndex == mCurItemIndex ? 10 : 26);
    canvas->drawBitmap(mFirstItemIndex == 0 ? mBmpMarker : mBmpMarker2, 135, 6);
    canvas->drawBitmap(mBmpMarker, 135, 6 + 8);
    canvas->drawBitmap(mBmpMarker, 135, 6 + 16);
    canvas->drawBitmap(mFirstItemIndex + 2 < mMagicChain->getLearnNum() ? mBmpMarker2 : mBmpMarker, 135, 6 + 24);
}

void ScreenMagic::onKeyDown(int key)
{
    if (key == KEY_UP && mCurItemIndex > 0)
    {
        --mCurItemIndex;
        if (mCurItemIndex < mFirstItemIndex)
        {
            --mFirstItemIndex;
        }
        mToDraw = mNextToDraw = 0;

        for (unsigned index = 0; index < mStackLastToDraw.size(); index++)
        {
            mStackLastToDraw.pop();
        }
    }
    else if (key == KEY_DOWN && mCurItemIndex + 1 < mMagicChain->getLearnNum())
    {
        ++mCurItemIndex;
        if (mCurItemIndex >= mFirstItemIndex + ITEM_NUM)
        {
            ++mFirstItemIndex;
        }
        mToDraw = mNextToDraw = 0;

        for (unsigned index = 0; index < mStackLastToDraw.size(); index++)
        {
            mStackLastToDraw.pop();
        }
    }
    else if (key == KEY_PAGEDOWN)
    {
        int len = mMagicChain->getMagic(mCurItemIndex)->getMagicDescription().length();
        if (mNextToDraw < len)
        {
            mStackLastToDraw.push(mToDraw); // 保存旧位置
            mToDraw = mNextToDraw; // 更新位置
        }
    }
    else if (key == KEY_PAGEUP && mToDraw != 0)
    {
        if (!mStackLastToDraw.empty())
        {
            mToDraw = mStackLastToDraw.top();
            mStackLastToDraw.pop();
        }
    }
}

void ScreenMagic::onKeyUp(int key)
{
    if (key == KEY_ENTER)   // 回调接口
    {
        mOnItemSelectedListener->onItemSelected(mMagicChain->getMagic(mCurItemIndex));
    }
    else if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
}
