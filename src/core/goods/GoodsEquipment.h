#ifndef _GoodsEquipment_H_
#define _GoodsEquipment_H_

#include <iostream>

#include "ScriptResources.h"
#include "BaseGoods.h"

class Player;

/**
 * 01冠类，02衣类，03鞋类，04护甲类，05护腕类
 */
class GoodsEquipment: public BaseGoods
{
public:
    GoodsEquipment();
    virtual ~GoodsEquipment(){}
    virtual void setOtherData(char * buf, int offset);

    void putOn(Player *p);

    void takeOff(Player *p);

public:
    bool bFatherOfGoodsWeapon;

protected:
    int mMpMax; // 加真气上限
    int mHpMax; // 加生命上限
    int mdf; // 防御
    int mat; // 攻击
    int mlingli; // 灵力
    int mSpeed; // 身法
    int mBitEffect; // 0、0、0、0（07武器类此处为全体效果）、毒、乱、封、眠（影响免疫效果，07武器类为攻击效果）
    int mLuck; // 吉运
};

#endif
