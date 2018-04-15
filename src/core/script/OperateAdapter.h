#ifndef _OperateAdapter_h_
#define _OperateAdapter_h_

#include "Operate.h"
#include "Canvas.h"
class OperateAdapter: public Operate
{
public:
    virtual ~OperateAdapter(){}
    virtual bool update(long delta)
    {
        return false;
    }

    virtual void draw(Canvas *canvas)
    {
    }

    virtual void onKeyDown(int key)
    {
    }

    virtual void onKeyUp(int key)
    {
    }

};

#endif
