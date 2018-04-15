#ifndef _GoodsTudun_H_
#define _GoodsTudun_H_

#include "BaseGoods.h"

/**
 * 13土遁类
 * 各作用字段均无意义,起作用的程序由脚本编写者决定(目前作用是从迷宫中直接返回)。
 * 当使用土遁类道具时，会触发当前执行脚本的第255号事件。
 * @author Chen
 *
 */
class GoodsTudun: public  BaseGoods
{
public:
    virtual ~GoodsTudun(){}
protected:

    virtual void setOtherData(char *buf, int offset)
    {

    }

};
#endif
