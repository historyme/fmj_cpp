#ifndef _OperateDrawOnce_h_
#define _OperateDrawOnce_h_

#include "Operate.h"
#include "Canvas.h"

class OperateDrawOnce: public Operate
{
public:
    OperateDrawOnce() :
    drawCnt(0)
    {

    }
    virtual ~OperateDrawOnce()
    {

    }
    
    virtual void drawOnce(Canvas *canvas) = 0;

    virtual bool update(long delta)
    {
        if (drawCnt >= 3)
        {
            drawCnt = 0;
            return false;
        }
        return true;
    }

    virtual void draw(Canvas *canvas)
    {
        drawOnce(canvas);
        ++drawCnt;
    }

    virtual void onKeyDown(int key)
    {
    }

    virtual void onKeyUp(int key)
    {
    }
    
    int drawCnt;
};

#endif