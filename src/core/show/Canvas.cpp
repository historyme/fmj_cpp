#include "Canvas.h"
#include "GameView.h"

Canvas::Canvas(Bitmap *b)
    :m_bitmap(b), m_need_delete(false)
{
}

Canvas::Canvas(int x, int y)
    : m_need_delete(true), m_bitmap(NULL)
{
    if (x > 0 && y>0)
    {
        m_bitmap = new Bitmap(x, y, ARGB_8888);
    }
}

Canvas::~Canvas()
{
    if (m_need_delete && m_bitmap != NULL)
    {
        delete m_bitmap;
    }
}

void Canvas::drawBitmap(Bitmap *bitmap, int left, int top)
{
    //将需要画的图片绘制在当前m_bitmap上
    Manager *m = Manager::getInstance();
    m->getMethod()->DrawBitmap(m_bitmap->handle,bitmap->handle, left, top);
}

void Canvas::drawColor(int color)
{
    //填充颜色
    Manager *m = Manager::getInstance();
    m->getMethod()->FillColor(m_bitmap->handle, color);
}


void Canvas::drawLine(int startX, int startY, int stopX, int stopY, Paint *paint)
{
    Manager *m = Manager::getInstance();
    int color;
    if (NULL == paint)
    {
        color = Manager::sCOLOR_BLACK;
    }
    else
    {
        color = paint->cOLOR_BLACK;
    }

    m->getMethod()->DrawLine(m_bitmap->handle, startX, startY, stopX, stopY, color);
}

/*
void Canvas::drawR(int x, int y, int i, int j, Paint sBlackPaint, Graphics g)
{
    if (sBlackPaint.style == Paint.Style.FILL){
        g.fillRect(x, y, i, j);
    }
    else if (sBlackPaint.style == Paint.Style.STROKE){
        g.drawRect(x, y, i, j);
    }
    else {
        g.fillRect(x, y, i, j);
    }
}
*/
void Canvas::drawRect(int x, int y, int i, int j, Paint *sBlackPaint)
{
    RectF rWithPic((float)x, (float)y, (float)i, (float)j);
    drawRect(&rWithPic, sBlackPaint);
}



void Canvas::drawRect(RectF *rWithPic, Paint *paint)
{
    int left = (int)(rWithPic->left);
    int top = (int)(rWithPic->top);
    int right = (int)(rWithPic->right);
    int bottom = (int)(rWithPic->bottom);

    Manager *m = Manager::getInstance();
    int color;
    if (NULL == paint)
    {
        color = Manager::sCOLOR_BLACK;
        m->getMethod()->DrawRect(m_bitmap->handle, left, top, right, bottom, color);
    }
    else
    {
        color = paint->cOLOR_BLACK;

        if (paint->style == Paint::FILL)
        {
            m->getMethod()->FillRect(m_bitmap->handle, left, top, right, bottom, color);
        }
        else if (paint->style == Paint::STROKE)
        {
            m->getMethod()->DrawRect(m_bitmap->handle, left, top, right, bottom, color);
        }
        else if (paint->style == Paint::FILL_AND_STROKE)
        {
            m->getMethod()->FillRect(m_bitmap->handle, left, top, right, bottom, color);

            m->getMethod()->DrawRect(m_bitmap->handle, left, top, right, bottom, color);
        }
        else
        {
            //没了
        }
    }
}

void Canvas::drawRect(Rect *mRectTop, Paint *mFramePaint)
{
    RectF rWithPic((float)mRectTop->left, (float)mRectTop->top, (float)mRectTop->right, (float)mRectTop->bottom);
    drawRect(&rWithPic, mFramePaint);
}


void Canvas::drawLines(int pts[], int size, Paint *sBlackPaint)
{
    for (int i = 0; i < size; i++)
    {
        drawLine(pts[i * 4], pts[(i * 4) + 1], pts[(i * 4) + 2], pts[((i * 4)) + 3]);
    }
}
