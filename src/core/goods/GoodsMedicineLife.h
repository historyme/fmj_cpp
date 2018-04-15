#ifndef _GoodsMedicineLife_H_
#define _GoodsMedicineLife_H_

#include <iostream>

class Player;

/**
 * 10灵药类
 * 对生命的恢复0~100,表示恢复被使用者??%的生命，
 * 并解除死亡状态，但被使用者必须是死亡状态。
 * @author Chen
 *
 */
class GoodsMedicineLife: public BaseGoods
{
public:
    virtual ~GoodsMedicineLife(){}

protected:
    virtual void setOtherData(char *buf, int offset)
    {
        mPercent = buf[offset + 0x17] & 0xff;
        if (mPercent > 100)
        {
            mPercent = 100;
        }
    }

private:
    int mPercent; // 恢复百分比


public:
    virtual void eat(Player *player)
    {
        player->setMP(player->getMP() + player->getMaxMP() * mPercent / 100);
        if (player->getMP() > player->getMaxMP())
        {
            player->setMP(player->getMaxMP());
        }
        player->sGoodsList->deleteGoods(mType, mIndex);
    }
};

#endif
