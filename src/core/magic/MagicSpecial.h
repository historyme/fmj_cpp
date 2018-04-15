#ifndef _MagicSpecial_H_
#define _MagicSpecial_H_


#include "BaseMagic.h"

/**
* 05特殊型 妙手空空
* @author Chen
*
*/
class  MagicSpecial : public BaseMagic
{
public:
    MagicSpecial();
protected:
    void setOtherData(char * buf, int offset)
    {
    }
};

#endif