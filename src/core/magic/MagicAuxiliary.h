#pragma once

#include "ResBase.h"
#include "BaseMagic.h"

/**
 * 04辅助型
 * @author Chen
 *
 */
class  MagicAuxiliary : public BaseMagic
{
protected:
    virtual void setOtherData(char * buf, int offset)
    {
        mHp = get2BytesInt(buf, offset + 0x12);
    }
private:
    int mHp;//0~100，表示被施展者恢复生命的百分比（起死回生）
};
