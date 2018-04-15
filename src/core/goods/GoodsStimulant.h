#ifndef _GoodsStimulant_H_
#define _GoodsStimulant_H_

#include "BaseGoods.h"

/**
 * 12兴奋剂
 * @author Chen
 *
 */
class GoodsStimulant: public BaseGoods
{
public:
    virtual ~GoodsStimulant(){}

protected :
    virtual void setOtherData(char *buf, int offset)
    {
        mdfPercent = (int)buf[offset + 0x18] & 0xff;
        matPercent = (int)buf[offset + 0x19] & 0xff;
        mSpeedPercent = (int)buf[offset + 0x1b] & 0xff;
        mForAll = ((int)buf[offset + 0x1c] & 0x10) != 0;
    }

public:
    virtual bool effectAll()
    {
        return mForAll;
    }

private:
    int mdfPercent;
    int matPercent;
    int mSpeedPercent;
    bool mForAll;

};
#endif