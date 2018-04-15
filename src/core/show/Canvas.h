#ifndef _Canvas_h_
#define _Canvas_h_

#include <iostream>
#include "Bitmap.h"
#include "Global.h"

#include "Paint.h"
#include "Manager.h"
#include "RectF.h"

class Canvas
{
public:
    Canvas(Bitmap *b);
    Canvas(int x, int y);
    virtual ~Canvas();
    void drawBitmap(Bitmap *bitmap, int left, int top);
    void drawColor(int color);
    void drawLine(int startX, int startY, int stopX, int stopY, Paint *paint = NULL);
    void drawRect(int x, int y, int i, int j, Paint *sBlackPaint = NULL);
    void drawRect(RectF *rWithPic, Paint *paint = NULL);
//    void drawR(int x, int y, int i, int j, Paint sBlackPaint, Graphics g);
    void drawRect(Rect *mRectTop, Paint *mFramePaint);
    void drawLines(int pts[],int size, Paint *sBlackPaint);
    void setBitmap(Bitmap *bmpArrowDown){ m_bitmap = bmpArrowDown; }
    Bitmap *getBitmap()const{ return m_bitmap; }

private:
    bool m_need_delete;
    //ª≠∞Â…œµƒ÷Ω
    Bitmap *m_bitmap;
};


#endif _Canvas_h_